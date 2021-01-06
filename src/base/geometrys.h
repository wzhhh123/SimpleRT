#pragma once

#include "base/header.h"
#include "base/shape.h"
#include "base/accelerate.h"

class Geometrys {

public:
	static Geometrys* Instance();

	std::vector<Shape*> shapes;

	bool Intersect(Ray r, IntersectPoint* p);

	void Initialize(std::vector<Model*>& models, std::vector<dMat4>& objectToWorlds);

	Accelerate* accelerater;


private:
	
	Geometrys();
	bool isInit;
};