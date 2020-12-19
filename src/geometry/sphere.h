#pragma once
#include "base/shape.h"


class Sphere : public Shape {

public:
	
	bool Intersect(Ray r, FLOAT* t) override;

	glm::vec3 center, color;
	double radius, kd, ks;
};