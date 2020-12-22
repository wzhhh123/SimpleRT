#pragma once
#include "header.h"
#include "ray.h"



struct IntersectPoint {

	FLOAT t;
	FLOAT u;
	FLOAT v;

};

class Shape {

public:
	
	virtual bool Intersect(Ray r, IntersectPoint& p) = 0;

};