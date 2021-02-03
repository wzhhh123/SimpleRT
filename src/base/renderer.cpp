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

void Renderer::Run()
{

	int yx = 0;
	while (yx < SIZE * SIZE) {
	
		dVec3 col = { 0,0,0 };
		
		int cnt = 0;
		//for (int i = 0; i < SPP; ++i) {
		for (int i = 0; i < 5; ++i) {

			float offsetX = rng.nextDouble() - 0.5;
			float offsetY = rng.nextDouble() - 0.5;

			dVec3 dir;
			dir.x = yx % SIZE - SIZE / 2 + offsetX;
			dir.y = SIZE / 2 - yx / SIZE + offsetY;
			dir.z = SIZE / (tan(AOV * acos(-1) / 360) * 2); // 360/PI~=114  计算近平面离相机距离 //照顾一下aabb计算 使用左手系

			Ray r = {};
			r.origin = dVec3{ 0,0,0 };
			r.direction = glm::normalize(dir);


			dVec3 temp = raytracer->Trace(DEPTH, r) * 255.0;
			if (temp.x > 1e-6 || temp.y > 1e-6 || temp.z > 1e-6) {
				col += temp;
				++cnt;
			}
		}

		col /= cnt;
		imageData[yx * CHANNEL_COUNT] = glm::clamp(col.x, 0.0, 255.0);
		imageData[yx * CHANNEL_COUNT + 1] = glm::clamp(col.y, 0.0, 255.0);
		imageData[yx * CHANNEL_COUNT + 2] = glm::clamp(col.z, 0.0, 255.0);

		++yx;

	}

	SaveImage(SIZE, SIZE, CHANNEL_COUNT, imageData);

	//DataToImage(imageData);

}


void Renderer::Initialize() {

	imageData = (unsigned char*) new char[SIZE*SIZE * CHANNEL_COUNT];

	using namespace std;
	using namespace rapidjson;

	string line, jsonText;
	ifstream ifs(CONFIG_PATH);
	while (getline(ifs, line))
		jsonText.append(line);//没有换行

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
		objectToWorldMats[index] = scale;

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
