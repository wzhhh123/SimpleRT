#pragma once
#include "header.h"
#include "ray.h"



struct IntersectPoint {

	FLOAT t;
	FLOAT weightU;
	FLOAT weightV;
	dVec3 normalWS;
	dVec3 normalOS;
	int shapeIndex; //index of geometry::shape array
	int modelIndex;
	int meshIndex;
};

class Shape {

public:
	
	virtual bool Intersect(Ray r, IntersectPoint& p) = 0;

};