#include "header.h"
#include "renderer.h"
#include "datatoimage.h"
#include "glm.hpp"
#include "ray.h"
#include "geometrys.h"
#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"
#include "geometry/triangle.h"
#include "tool/imagehelper.h"
#include "rapidjson/document.h"
#include "rapidjson/stringbuffer.h"
#include <tbb/parallel_for.h>
#include <tbb/blocked_range.h>
#include <tbb/task_scheduler_init.h>
#include "tool/timer.h"
#include "tool/math.h"

void RenderTile(int tileIndex) {
	int offset = SIZE * SIZE / THREAD_COUNT;
	int left = offset * tileIndex;
	int right = offset * (tileIndex + 1);
	if (tileIndex == THREAD_COUNT - 1) right = SIZE * SIZE;
	std::string  str = std::to_string(left) + " " + std::to_string(right) + " rendering!\n";
	std::cout << str;
	std::cout.flush();
	pcg32 rng;
	rng.seed(8, 36);
	int yx = left;
	float dirZ = SIZE / (tan(AOV * acos(-1) / 360) * 2); // 360/PI~=114  �����ƽ����������� //�չ�һ��aabb���� ʹ������ϵ
	while (yx < right) {
		dVec3 col = { 0,0,0 };
		int cnt = 0;
		//for (int i = 0; i < SPP; ++i) {
		for (int i = 0; i < 1; ++i) {

			float offsetX = rng.nextDouble() - 0.5;
			float offsetY = rng.nextDouble() - 0.5;

			offsetY = offsetX = 0;

			dVec3 dir;
			dir.x = yx % SIZE - SIZE / 2 + offsetX;
			dir.y = SIZE / 2 - yx / SIZE + offsetY;
			dir.z = dirZ;

			Ray r = {};
			r.origin = dVec3{ 0,0,0 };
			r.direction = glm::normalize(dir);

			dVec3 temp = Renderer::Instance()->raytracer->Trace(DEPTH, r);
			if (temp.x > 1e-6 || temp.y > 1e-6 || temp.z > 1e-6) {
				col += temp;
				++cnt;
			}
		}

		col /= cnt;
		col = toSRGB(col) * 255.0;
		//col *= 255.0;

		Renderer::Instance()->imageData[yx * CHANNEL_COUNT] = glm::clamp(col.x, 0.0, 255.0);
		Renderer::Instance()->imageData[yx * CHANNEL_COUNT + 1] = glm::clamp(col.y, 0.0, 255.0);
		Renderer::Instance()->imageData[yx * CHANNEL_COUNT + 2] = glm::clamp(col.z, 0.0, 255.0);

		++yx;
	}
}

void Renderer::Run()
{

	std::thread render_thread([&] {
		tbb::task_scheduler_init init(THREAD_COUNT);

		std::cout << "Rendering .. \n";
		std::cout.flush();
		Timer timer;
		tbb::blocked_range<int> range(0, THREAD_COUNT);


		auto map = [&](const tbb::blocked_range<int> &range) {
			/* Allocate memory for a small image block to be rendered
			   by the current thread */

			//std::cout << range.end() << std::endl;
			//begin��end���Զ����ӣ���
			RenderTile(range.begin());

			/* Create a clone of the sampler for the current thread */
			//RunTile(range.begin());
			//for (int i = range.begin(); i < range.end(); ++i) {
				//renderBlock(scene, sampler.get(), block);

		};
		/// Default: parallel rendering
		tbb::parallel_for(range, map);

		std::cout << "done. (took " << timer.elapsedString() << ")" << std::endl;
	});


	/* Shut down the user interface */
	render_thread.join();

	std::cout << "save" << std::endl;
	SaveImage(SIZE, SIZE, CHANNEL_COUNT, imageData);

	//DataToImage(imageData);

}


void Renderer::Initialize() {

	imageData = (unsigned char*) new char[SIZE * SIZE * CHANNEL_COUNT];

	using namespace std;
	using namespace rapidjson;

	string line, jsonText;
	ifstream ifs(CONFIG_PATH);
	while (getline(ifs, line))
		jsonText.append(line);//û�л���

	Document document;
	document.Parse(jsonText.c_str());
	ifs.close();


	const auto &objs = document["scene"].GetArray();
	models.resize(objs.Size());
	objectToWorldMats.resize(objs.Size());
	int index = 0;
	for (const auto &obj : objs)
	{
		std::string path = obj["path"].GetString();

		auto position = obj["position"].GetArray().Begin();
		dMat4 trans = glm::translate(dMat4(1.0f), dVec3(position->GetFloat(), (position + 1)->GetFloat(), (position + 2)->GetFloat()));

		auto rotate = obj["euler"].GetArray().Begin();
		dVec3 euler = dVec3(rotate->GetFloat(), (rotate + 1)->GetFloat(), (rotate + 2)->GetFloat());

		dMat4 rotation = glm::rotate(trans, eulerToRadius(euler.y), dVec3{ 0,1,0 });
		rotation = glm::rotate(rotation, eulerToRadius(euler.x), dVec3{ 1,0,0 });
		rotation = glm::rotate(rotation, eulerToRadius(euler.z), dVec3{ 0,0,1 });

		auto scalec = obj["scale"].GetArray().Begin();
		dMat4 scale = glm::scale(rotation, dVec3(scalec->GetFloat(), (scalec + 1)->GetFloat(), (scalec + 2)->GetFloat()));

		dMat4 worldToView = getViewMatrixRTL(dVec3{ 0,0,0 }, dVec3{ 0,0,-1 }, dVec3{ 0,1,0 });

		auto res = worldToView * dVec4(0, -0.8, -2, 1);
		 std::cout << res.x  << " " << res.y << " " << res.z << std::endl;

		 //exit(0);
		 objectToWorldMats[index] = worldToView * scale;

		models[index] = new Model();
		models[index]->Initialize(path.c_str());
		++index;
	}

	{
		Geometrys::Instance()->Initialize(models, objectToWorldMats);
		std::cout << "init geometry done!" << std::endl;
	}

}


glm::vec4 Renderer::GetAmbient(int modelIndex, int meshIndex) {
	return models[modelIndex]->meshes[meshIndex].ambient;
}

glm::vec4 Renderer::GetDiffuse(int modelIndex, int meshIndex) {
	return models[modelIndex]->meshes[meshIndex].diffuse;
}


BxDF* Renderer::GetBxDF(int modelIndex, int meshIndex) {
	return models[modelIndex]->meshes[meshIndex].bxdf;
}


glm::vec4 Renderer::GetEmissive(int modelIndex, int meshIndex) {
	return models[modelIndex]->meshes[meshIndex].emissive;
}


glm::vec4 Renderer::GetShininess(int modelIndex, int meshIndex) {
	return models[modelIndex]->meshes[meshIndex].shininess;
}


Renderer* Renderer::Instance() {
	static Renderer* ins = new Renderer();
	return ins;
}

void Renderer::deIntialize() {

	delete[] imageData;
}

Renderer::Renderer() {

	rng.seed(2, 56);


}
