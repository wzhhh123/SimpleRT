#pragma once

#include "header.h"

#include "geometry/sphere.h"
#include "raytracer.h"
class Renderer {

public:
	
	void Initialize();
	void Run();
	void deIntialize();

	Sphere s;
	glm::dvec3 *imageData;
	RayTracer* raytracer;

	Renderer();

};