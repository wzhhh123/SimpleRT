#pragma once
#include "base/header.h"
#include "base/shape.h"

class Triangle : public Shape {

public:
	
	Triangle(dVec3 _v0, dVec3 _v1, dVec3 _v2);

	bool Intersect(Ray r, IntersectPoint& p) override;


	dVec3 v0;
	dVec3 v1;
	dVec3 v2;

};