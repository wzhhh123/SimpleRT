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

void Renderer::Run()
{

	int yx = 0;
	while (yx < SIZE * SIZE) {

		dVec3 dir;
		dir.x = yx % SIZE - SIZE / 2;
		dir.y = SIZE / 2 - yx / SIZE;
		dir.z = SIZE  / (tan(AOV * acos(-1) / 360) * 2); // 360/PI~=114  计算近平面离相机距离 //照顾一下aabb计算 使用左手系

		Ray r = {};
		r.origin = dVec3{ 0,0,0 };
		r.direction = glm::normalize(dir);
	
		auto col = raytracer->Trace(DEPTH, r) * 255.0;
		imageData[yx * CHANNEL_COUNT] = col.x < 0 ? 0 : (int)(col.x);
		imageData[yx * CHANNEL_COUNT + 1] = col.y < 0 ? 0 : (int)(col.y);
		imageData[yx * CHANNEL_COUNT + 2] = col.z < 0 ? 0 : (int)(col.z);

		++yx;

	}

	SaveImage(SIZE, SIZE, CHANNEL_COUNT, imageData);

	//DataToImage(imageData);

}


void Renderer::Initialize() {

	imageData = (unsigned char*) new char[SIZE*SIZE * CHANNEL_COUNT];

	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ -0.57, -1, 2.3698 }, VEC3{ 1., 1., 1. }, 0.5, .05, .2, .85, 0., 1.7));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ 0.63, -1, 2.369 }, VEC3{ 1., .5, .2 }, 0.5, .7, .3, 0., 0.05, 1.2));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ 0.01, 0.43, 3.15 }, VEC3{ .1, .8, .8 }, 1., .3, .7, 0., 0., 1.2));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ 3.67, 8.99, 5.64 }, VEC3{ 1., .8, 1. }, 3.5, 0., 0., 0., .6, 1.5));
	//Geometrys::Instance()->shapes.push_back(
	//	new Sphere(VEC3{ -5.02, 6.67, 5.97 }, VEC3{ .8, 1., 1. }, 2.5, 0., 0., 0., .5, 1.5));


	//Geometrys::Instance()->shapes.push_back(
	//	new Triangle(VEC3{ 0.5, -0.5,2}, VEC3{ 0.5, 0.5,2 }, VEC3{ -0.5, 0.5,2 }));
	//Geometrys::Instance()->shapes.push_back(
	//	new Triangle(VEC3{ -0.5, 0.5,2 }, VEC3{ -0.5, -0.5,2 }, VEC3{ 0.5, -0.5,2 }));


	//prepare scene data
	{
		models.resize(1);
		objectToWorldMats.resize(1);

		models[0] = new Model();
		models[0]->Initialize("../assets/models/Common_Bench_A.FBX");

		dMat4 trans = glm::translate(dMat4(1.0f), dVec3{ -0.30000001192092898, -0.800000011920929, 3.4000000953674318 });
		dMat4 rotation = glm::rotate(trans, eulerToRadius(54.20000076293945), dVec3{ 0,1,0 });
		rotation = glm::rotate(rotation, eulerToRadius(267.6000061035156), dVec3{ 1,0,0 });
		rotation = glm::rotate(rotation, eulerToRadius(0), dVec3{ 0,0,1 });
		dMat4 scale = glm::scale(rotation, dVec3{ 0.01,0.01,0.01 });
		objectToWorldMats[0] = scale;

		std::cout << "load models done!" << std::endl;
	}

	{
		Geometrys::Instance()->Initialize(models, objectToWorldMats);
		std::cout << "init geometry done!" << std::endl;
	}



}

void Renderer::deIntialize() {

	delete[] imageData;
}

Renderer::Renderer() {}
