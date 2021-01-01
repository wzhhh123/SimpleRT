#pragma once

#include "header.h"

#include "geometry/sphere.h"
#include "raytracer.h"
#include "asset/model.h"
#include "accelerate.h"
class Renderer {
public:
	
	void Initialize();
	void Run();
	void deIntialize();

	unsigned char* imageData;
	RayTracer* raytracer;
	std::vector<Model*>models;
	std::vector<dMat4> objectToWorldMats;

	Renderer();

};