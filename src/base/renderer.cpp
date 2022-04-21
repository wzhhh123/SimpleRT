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
#include "materials/glass.h"
#include "camera.h"
#include "tool/platform.h"
#include "texture.h"
#include "samplers/halton.h"
#include "filters/gaussian.h"
#include "film.h"
#include "filters/box.h"
#include <atomic>
//#include "ImfRgbaFile.h"
//#include "ImfArray.h"
//#include "namespaceAlias.h"


float FOV = 45.0f;
int SPP = 2;


float Halton(int index, int radix)
{
   float result = 0;
   float fraction = 1.0 / radix;

   while (index > 0)
   {
       result += (index % radix) * fraction;

       index /= radix;
       fraction /= radix;
   }
   return result;
}

inline void WriteToBuffer(Point2i& p, dVec3& col)
{
	int Idx = p.x + p.y * IMG_SIZE;
	Renderer::Instance()->imageData[Idx * CHANNEL_COUNT] = col.x;
	Renderer::Instance()->imageData[Idx * CHANNEL_COUNT + 1] = col.y;
	Renderer::Instance()->imageData[Idx * CHANNEL_COUNT + 2] = col.z;
}

void RenderTile(Point2i tileMin, Point2i tileMax, std::shared_ptr<Film> film, int tileNum, std::shared_ptr<Sampler> sampler) {


	static std::atomic<int>tileIdx(0);
	std::string  str = std::to_string(1+tileIdx++) + "/" + std::to_string(tileNum) + " " + std::to_string(tileMin.x) + " " + std::to_string(tileMin.y) + "," + std::to_string(tileMax.x) + " " + std::to_string(tileMax.y) + " rendering!\n";
	std::cout << str;
	std::cout.flush();

	pcg32 rng;
	rng.seed(8, 36);

	Camera Cam;
	int localSPP = SPP;

	std::shared_ptr<GaussianFilter> filter = std::shared_ptr<GaussianFilter>(new GaussianFilter({ 2,2 }, 2));
	//std::shared_ptr<BoxFilter> filter = std::shared_ptr<BoxFilter>(new BoxFilter({0.5,0.5}));
	Bound2i tileSampleBound = Bound2i(tileMin, tileMax);
	std::shared_ptr<FilmTile>filmTile = film->GetFilmTile(tileSampleBound);

	for (int i = tileMin.x; i < tileMax.x; ++i)
	{
		for (int j = tileMin.y; j < tileMax.y; ++j)
		{
			sampler->StartPixel({ i,j });
			dVec3 col = { 0,0,0 };
			int cnt = 0;
			FLOAT weightSum = 0;
			do{
				Ray r = {};

				dVec2 offset = sampler->Get2D();
				Cam.GenerateRay(Point2i(i, j), offset, r);

				//float offsetX = rng.nextDouble();
				//float offsetY = rng.nextDouble();
				//Cam.GenerateRay(Point2i(i, j), {offsetX, offsetY}, r);

				dVec3 L = Renderer::Instance()->raytracer->Trace(DEPTH, r, sampler);
				filmTile->AddSample(dVec2(i, j) + offset, L);

			} while (sampler->StartNextSample());
           
			// Point2i p = Point2i(i,j);
			// WriteToBuffer(p, col / weightSum);
			// WriteToBuffer(p, col);

		}
	}

	film->MergeTile(filmTile);
}

void Renderer::RunHaltonSample()
{
	GlobalSampler* sampler = new HaltonSampler(30, Bound2i({ 0,0 }, { IMG_SIZE/10, IMG_SIZE / 10 }));
	for (int i = 0; i < IMG_SIZE / 10; ++i)
	{
		for (int j =  0; j < IMG_SIZE / 10; ++j)
		{
			sampler->StartPixel({ i,j });
			dVec3 col = { 1,1,1 };
			int cnt = 0;
			do {
				dVec2 offset = sampler->Get2D();
			//	std::cout << offset.x << " " << offset.y << std::endl;
			//	std::cout << std::flush;
				float x = i * 1.0f + offset.x;
				float y = j * 1.0f + offset.y;
                Point2i p = Point2i(x*10, y*10);
				WriteToBuffer(p, col);

			} while (sampler->StartNextSample());
		}
	}
	EXR_HELPER::SaveAsExrFile(OUTPUT_PATH_EXR, IMG_SIZE, IMG_SIZE, imageData);
	return;


	for (int i = 0; i < 100000; ++i)
	{
		float x = Halton(i, 2);
		float y = Halton(i, 3);
		int index = (int)(x*IMG_SIZE)*IMG_SIZE + (int)(y*IMG_SIZE);
		Renderer::Instance()->imageData[index * CHANNEL_COUNT] = 1;
		Renderer::Instance()->imageData[index * CHANNEL_COUNT + 1] = 1;
		Renderer::Instance()->imageData[index * CHANNEL_COUNT + 2] = 1;
	}

	EXR_HELPER::SaveAsExrFile(OUTPUT_PATH_EXR, IMG_SIZE, IMG_SIZE, imageData);
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
    
    
	//std::unique_ptr<Filter> filter = std::unique_ptr<Filter>(new GaussianFilter({ 2,2 }, 2));
	std::unique_ptr<Filter> filter = std::unique_ptr<Filter>(new BoxFilter({ 0.5,0.5 }));
    std::shared_ptr<Film> film = std::shared_ptr<Film>(new Film(Point2i(IMG_SIZE, IMG_SIZE), std::move(filter), OUTPUT_PATH_EXR));
	std::shared_ptr<HaltonSampler> sampler = std::shared_ptr<HaltonSampler>(new HaltonSampler(SPP, film->GetSampleBound()));

	std::thread render_thread([&] {
		tbb::task_scheduler_init init(THREAD_COUNT);

		std::cout << "Rendering .. \n";
		std::cout.flush();
		Timer timer;

		const int tileSize = 16;
        
        Bound2i sampleBound = film->GetSampleBound();
        Point2i sampleBoundExtent = {sampleBound.max.x - sampleBound.min.x, sampleBound.max.y - sampleBound.min.y};
		Point2i nTiles((sampleBoundExtent.x + tileSize - 1) / tileSize, (sampleBoundExtent.y + tileSize - 1) / tileSize);
        Point2i offset = {0-sampleBound.min.x, 0-sampleBound.min.y};
        
		std::shared_ptr<Sampler>tileSampler = sampler->Clone();

		tbb::blocked_range<int> range(0, nTiles.x* nTiles.y);
        
		auto map = [&](const tbb::blocked_range<int> &range) {
		
			int tileIdx = range.begin();
			Point2i tileMin = Point2i(tileIdx % nTiles.y, tileIdx / nTiles.y) * tileSize;
			Point2i tileMax = Point2i(std::min(tileMin.x + tileSize, sampleBoundExtent.x), std::min(tileMin.y + tileSize, sampleBoundExtent.y));
            
            tileMin -= offset;
            tileMax -= offset;


            RenderTile(tileMin, tileMax, film, nTiles.x * nTiles.y, tileSampler);

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

	film->SaveFilm();
    //unsigned char* pixels = new unsigned char[IMG_SIZE*IMG_SIZE*CHANNEL_COUNT];
    //for(int i = 0; i < IMG_SIZE*IMG_SIZE*CHANNEL_COUNT; ++i)
    //{
    //    pixels[i] = (char)std::floor(fastToneMap(imageData[i]) * 255);
    //}
	//SaveImage(IMG_SIZE, IMG_SIZE, CHANNEL_COUNT, pixels);
    //delete []pixels;
    
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
		jsonText.append(line);//û�л���

	Document document;
	document.Parse(jsonText.c_str());
	ifs.close();

	const auto &camera = document["camera"].GetObject();
	auto cameraPos = camera["position"].GetArray().Begin();
	dVec3 pos = glm::vec3(cameraPos->GetFloat(), (cameraPos + 1)->GetFloat(), (cameraPos + 2)->GetFloat());
	auto lookTarget = camera["target"].GetArray().Begin();
	dVec3 target = glm::vec3(lookTarget->GetFloat(), (lookTarget + 1)->GetFloat(), (lookTarget + 2)->GetFloat());
	FOV = camera["fov"].GetFloat();

	const auto& renderer = document["renderer"].GetObject();
	SPP = renderer["spp"].GetInt();

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
