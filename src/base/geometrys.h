#pragma once

#include "base/header.h"
#include "base/shape.h"
#include "base/accelerate.h"
#include "sampling.h"

class Geometrys {

public:
	static Geometrys* Instance();

	std::vector<Shape*> shapes;
	std::vector<Shape*>lights;

	bool Intersect(Ray r, IntersectPoint* p);

	void Initialize(std::vector<Model*>& models, std::vector<dMat4>& objectToWorlds);

	Accelerate* accelerater;


	Distribution1D lightDistribute;
	std::vector<int>lightShapeIndices;

private:
	
	Geometrys();
	bool isInit;
};