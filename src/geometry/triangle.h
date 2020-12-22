#pragma once
#include "base/header.h"
#include "base/shape.h"

class Triangle : public Shape {

public:
	
	Triangle(VEC3 _v0, VEC3 _v1, VEC3 _v2);

	bool Intersect(Ray r, IntersectPoint& p) override;


	VEC3 v0;
	VEC3 v1;
	VEC3 v2;

};