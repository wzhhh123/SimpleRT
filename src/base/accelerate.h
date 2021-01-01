#pragma once

#include "header.h"

class Accelerate {


public:

	virtual void Initialize(std::vector<Shape*>*scene) = 0;
	virtual bool Intersect(Ray& r, IntersectPoint* p, int* index) = 0;

};
