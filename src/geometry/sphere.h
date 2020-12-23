#pragma once
#include "base/shape.h"


class Sphere : public Shape {

public:
	
	Sphere();

	bool Intersect(Ray r, IntersectPoint& p) override;

	Sphere(dVec3 _center, dVec3 _color, FLOAT _radius,
		FLOAT _kd, FLOAT _ks, FLOAT _kt, FLOAT _kl, FLOAT _ir);

	dVec3 center, color;
	FLOAT radius, kd, ks, kt, kl, ir;
};