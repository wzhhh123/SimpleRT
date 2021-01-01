#pragma once

#include "base/header.h"
#include "base/accelerate.h"

class Linear : public Accelerate {


public:

	virtual void Initialize(std::vector<Shape*>*scene);
	virtual bool Intersect(Ray& r, IntersectPoint* p, int* index);

	std::vector<Shape*>* scene;

};
