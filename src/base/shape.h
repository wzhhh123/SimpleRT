#pragma once
#include "header.h"
#include "ray.h"

class Shape {

public:
	
	virtual bool Intersect(Ray r, FLOAT* t) = 0;



};