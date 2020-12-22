#include "header.h"
#include "renderer.h"
#include "datatoimage.h"
#include "glm.hpp"
#include "ray.h"
#include "geometrys.h"

void Renderer::Run()
{

	int yx = 0;
	while (yx < SIZE * SIZE) {

		glm::dvec3 dir;
		dir.x = yx % SIZE - SIZE / 2;
		dir.y = SIZE / 2 - yx / SIZE;
		dir.z = SIZE  / (tan(AOV * acos(-1) / 180) * 2); // 360/PI~=114  计算近平面离相机距离

		Ray r = {};
		r.origin = glm::dvec3{ 0,0,0 };
		r.direction = glm::normalize(dir);
	
		imageData[yx] = raytracer->Trace(DEPTH, r) * 255.0;
		++yx;
	}

	DataToImage(imageData);

}


void Renderer::Initialize() {

	imageData = new glm::dvec3[SIZE*SIZE];

	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ -0.57, -1, 2.3698 }, glm::dvec3{ 1., 1., 1. }, 0.5, .05, .2, .85, 0., 1.7));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ 0.63, -1, 2.369 }, glm::dvec3{ 1., .5, .2 }, 0.5, .7, .3, 0., 0.05, 1.2));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ 0.01, 0.43, 3.15 }, glm::dvec3{ .1, .8, .8 }, 1., .3, .7, 0., 0., 1.2));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ 3.67, 8.99, 5.64 }, glm::dvec3{ 1., .8, 1. }, 3.5, 0., 0., 0., .6, 1.5));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ -5.02, 6.67, 5.97 }, glm::dvec3{ .8, 1., 1. }, 2.5, 0., 0., 0., .5, 1.5));

}

void Renderer::deIntialize() {

	delete[] imageData;
}

Renderer::Renderer() {}
