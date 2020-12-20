#pragma once
#include "base/shape.h"


class Sphere : public Shape {

public:
	
	Sphere();

	bool Intersect(Ray r, FLOAT* t) override;

	Sphere(glm::dvec3 _center, glm::dvec3 _color, FLOAT _radius,
		FLOAT _kd, FLOAT _ks, FLOAT _kt, FLOAT _kl, FLOAT _ir);

	glm::dvec3 center, color;
	FLOAT radius, kd, ks, kt, kl, ir;
};