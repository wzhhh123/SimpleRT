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
		dir.z = SIZE / 2 - yx / SIZE;
		dir.y = SIZE / 2 / tan(AOV / 114.5915590261); // 360/PI~=114  计算近平面离相机距离

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

	s.center = glm::dvec3{ 0., 10., 0 };
	s.radius = 1;

	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ 0., 6., .5 }, glm::dvec3{ 1., 1., 1. }, .9, .05, .2, .85, 0., 1.7));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ -1., 8., -.5 }, glm::dvec3{ 1., .5, .2 }, 1., .7, .3, 0., .05, 1.2));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ 1., 8., -.5 }, glm::dvec3{ .1, .8, .8 }, 1., .3, .7, 0., 0., 1.2));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ 3., -6., 15. }, glm::dvec3{ 1., .8, 1. }, 7., 0., 0., 0., .6, 1.5));
	Geometrys::Instance()->shapes.push_back(
		new Sphere(glm::dvec3{ -3., -3., 12. }, glm::dvec3{ .8, 1., 1. }, 5., 0., 0., 0., .5, 1.5));

	
}

void Renderer::deIntialize() {

	delete[] imageData;
}

Renderer::Renderer() {}
