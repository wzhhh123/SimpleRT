#pragma once
#include "base/shape.h"


class Sphere : public Shape {

public:
	
	Sphere();

	bool Intersect(Ray r, IntersectPoint& p) override;

	Sphere(VEC3 _center, VEC3 _color, FLOAT _radius,
		FLOAT _kd, FLOAT _ks, FLOAT _kt, FLOAT _kl, FLOAT _ir);

	VEC3 center, color;
	FLOAT radius, kd, ks, kt, kl, ir;
};