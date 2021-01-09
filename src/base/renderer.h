#pragma once

#include "header.h"

#include "geometry/sphere.h"
#include "raytracer.h"
#include "asset/model.h"
#include "accelerate.h"
#include "pcg32.h"
#include "bxdf.h"
#include "bxdf/lambert.h"


class Renderer {
public:
	
	void Initialize();
	void Run();
	void deIntialize();

	unsigned char* imageData;
	RayTracer* raytracer;
	std::vector<Model*>models;
	std::vector<dMat4> objectToWorldMats;

	static Renderer* Instance();

	glm::vec4 GetAmbient(int modelIndex, int meshIndex);
	glm::vec4 GetDiffuse(int modelIndex, int meshIndex);
	glm::vec4 GetEmissive(int modelIndex, int meshIndex);
	glm::vec4 GetShininess(int modelIndex, int meshIndex);


	pcg32 rng;
	BxDF* lambert;

private:
	Renderer();

};