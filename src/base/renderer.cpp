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
#include "tool/exrhelper.h"
#include "bxdf/glass.h"
#include "camera.h"
#include "tool/platform.h"
#include "texture.h"
//#include "ImfRgbaFile.h"
//#include "ImfArray.h"
//#include "namespaceAlias.h"
void RenderTile(int tileIndex) {
	int offset = IMG_SIZE * IMG_SIZE / THREAD_COUNT;
	int left = offset * tileIndex;
	int right = offset * (tileIndex + 1);
	if (tileIndex == THREAD_COUNT - 1) right = IMG_SIZE * IMG_SIZE;
	std::string  str = std::to_string(left) + " " + std::to_string(right) + " rendering!\n";
	std::cout << str;
	std::cout.flush();
	pcg32 rng;
	rng.seed(8, 36);
	int yx = left;
	Camera Cam;
	while (yx < right) {
		dVec3 col = { 0,0,0 };
		int cnt = 0;
		//for (int i = 0; i < SPP; ++i) {
		for (int i = 0; i < 8; ++i) {

			float offsetX = rng.nextDouble() - 0.5;
			float offsetY = rng.nextDouble() - 0.5;

			//offsetY = offsetX = 0;

			Ray r = {};
			Cam.GenerateRay(yx, dVec2{offsetX, offsetY}, r);
            //Cam.GenerateRay(yx, dVec2{0.0, 0.0}, r);

			dVec3 temp = Renderer::Instance()->raytracer->Trace(DEPTH, r);
			//if (temp.x > 1e-6 || temp.y > 1e-6 || temp.z > 1e-6) {
				col += temp;
				++cnt;
			//}
		}

		col /= cnt;
		//col = toSRGB(col) * 255.0;
		//col *= 255.0;
		

		//Renderer::Instance()->imageData[yx * CHANNEL_COUNT] = glm::clamp(col.x, 0.0, 255.0);
		//Renderer::Instance()->imageData[yx * CHANNEL_COUNT + 1] = glm::clamp(col.y, 0.0, 255.0);
		//Renderer::Instance()->imageData[yx * CHANNEL_COUNT + 2] = glm::clamp(col.z, 0.0, 255.0);
            
        Renderer::Instance()->imageData[yx * CHANNEL_COUNT] = col.x;
        Renderer::Instance()->imageData[yx * CHANNEL_COUNT + 1] = col.y;
        Renderer::Instance()->imageData[yx * CHANNEL_COUNT + 2] = col.z;
        
		++yx;
	}
}

void Renderer::Run()
{
    
    //FresnelSpecular specular(dVec3(1),dVec3(1),1,1.5);
    //dVec3 wi;
    //double pdf;
    //BxDFType type;
    //dVec3 f = specular.Sample_f(dVec3(0.3,0.3,0.91), &wi, {0,0}, &pdf, type);
    //std::cout << f.x <<" " << f.y <<" " << f.z <<", " << wi.x <<" " << wi.y <<" " << wi.z <<"\n";
    //std::cout.flush();
    //return;
    
    //dMat4 mat(0);
    //mat[0][0] = 1;
    //mat[1][1] = 1;
    //mat[2][2] = 1;
    //mat[3][3] = 1;
    
    //Triangle tri(dVec3(-1,-1,1), dVec3(1,-1,1), dVec3(1,5,1), mat,0,0,0);
    //Triangle tri(dVec3(-1,-1,1), dVec3(1,5,1), dVec3(1,-1,1), mat,0,0,0);
    //Ray r;
    //r.origin = dVec3(0,0,0);
    //r.direction = dVec3(0,0,1);
    //IntersectPoint p;
    //bool a = tri.Intersect(r, p);
    //std::cout << a <<"\n";
    //std::cout.flush();
    //return;
    
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
			//begin和end都自动叠加？？
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
	//SaveImage(IMG_SIZE, IMG_SIZE, CHANNEL_COUNT, imageData);
    
    EXR_HELPER::SaveAsExrFile(OUTPUT_PATH_EXR ,IMG_SIZE, IMG_SIZE, imageData);
    
    //IMF::Rgba r;
    //r.r = 1;
    

	//DataToImage(imageData);

}

void Renderer::Initialize() {

	//imageData = (unsigned char*) new char[IMG_SIZE * IMG_SIZE * CHANNEL_COUNT];
    imageData = new float[IMG_SIZE * IMG_SIZE * CHANNEL_COUNT];
    
	using namespace std;
	using namespace rapidjson;

	string line, jsonText;
	ifstream ifs(CONFIG_PATH);
	while (getline(ifs, line))
		jsonText.append(line);//没有换行

	Document document;
	document.Parse(jsonText.c_str());
	ifs.close();

	const auto &camera = document["camera"].GetObject();
	auto cameraPos = camera["position"].GetArray().Begin();
	dVec3 pos = glm::vec3(cameraPos->GetFloat(), (cameraPos + 1)->GetFloat(), (cameraPos + 2)->GetFloat());
	auto lookTarget = camera["target"].GetArray().Begin();
	dVec3 target = glm::vec3(lookTarget->GetFloat(), (lookTarget + 1)->GetFloat(), (lookTarget + 2)->GetFloat());
	dMat4 worldToView = getViewMatrixRTL(pos, target, dVec3{ 0,1,0 });

	const auto &objs = document["scene"].GetArray();
	models.resize(objs.Size());
	objectToWorldMats.resize(objs.Size());
	int index = 0;
	for (const auto &obj : objs)
	{
		std::string path = ConvertRelativePath(obj["path"].GetString());

		auto position = obj["position"].GetArray().Begin();
		dMat4 trans = glm::translate(dMat4(1.0f), dVec3(position->GetFloat(), (position + 1)->GetFloat(), (position + 2)->GetFloat()));

		auto rotate = obj["euler"].GetArray().Begin();
		dVec3 euler = dVec3(rotate->GetFloat(), (rotate + 1)->GetFloat(), (rotate + 2)->GetFloat());

		dMat4 rotation = glm::rotate(trans, eulerToRadius(euler.y), dVec3{ 0,1,0 });
		rotation = glm::rotate(rotation, eulerToRadius(euler.x), dVec3{ 1,0,0 });
		rotation = glm::rotate(rotation, eulerToRadius(euler.z), dVec3{ 0,0,1 });

		auto scalec = obj["scale"].GetArray().Begin();
		dMat4 scale = glm::scale(rotation, dVec3(scalec->GetFloat(), (scalec + 1)->GetFloat(), (scalec + 2)->GetFloat()));

		 objectToWorldMats[index] = worldToView * scale;

		models[index] = new Model();
		models[index]->Initialize(path.c_str());
		for (int i = 0; i < models[index]->meshes.size(); ++i) {
			if (obj.HasMember("radiance")) {
				models[index]->meshes[i].isAreaLight = true;
				auto radiance = obj["radiance"].GetArray().Begin();
				models[index]->meshes[i].emissive = dVec4(radiance->GetFloat(), (radiance + 1)->GetFloat(), (radiance + 2)->GetFloat(), 0);
                models[index]->meshes[i].AreaLight = CreateAreaLight(models[index]->meshes[i].emissive);
			}
			if (obj.HasMember("bsdf"))
            {
                auto albedo = obj["albedo"].GetArray().Begin();
                models[index]->meshes[i].ambient = dVec4{ albedo->GetFloat(), (albedo + 1)->GetFloat(), (albedo + 2)->GetFloat(), 1 };
				ImageTexture<dVec3, dVec3> *tex = nullptr;
				if (obj.HasMember("test_tex"))
				{
					std::string texPath = ConvertRelativePath(obj["test_tex"].GetString());
					tex = new ImageTexture<dVec3, dVec3>(texPath);
				}
                if(strcmp(obj["bsdf"].GetString(), "diffuse") == 0)
                {
                    models[index]->meshes[i].material = CreateMatteMaterial(models[index]->meshes[i].ambient, tex);
                }
                else if(strcmp(obj["bsdf"].GetString(), "glass") == 0)
                {
                    models[index]->meshes[i].material = CreateGlassMaterial(dVec3(1,1,1), dVec3(1,1,1), obj["index"].GetFloat());
                }
			}

		}
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
    return nullptr;
	//return models[modelIndex]->meshes[meshIndex].bxdf;
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
