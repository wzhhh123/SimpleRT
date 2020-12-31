#pragma once

#include "header.h"

#include "geometry/sphere.h"
#include "raytracer.h"
#include "asset/model.h"

class Renderer {
public:
	
	void Initialize();
	void Run();
	void deIntialize();

	unsigned char* imageData;
	RayTracer* raytracer;
	Model model;

	Renderer();

};