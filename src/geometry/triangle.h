#pragma once
#include "base/shape.h"


class Triangle : public Shape {

public:
	
	Triangle();

	bool Intersect(Ray r, FLOAT* t) override;

	
};