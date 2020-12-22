#pragma once
#include "base/shape.h"


//AABB BoundingBox
class BoundingBox : public Shape {

public:
	
	BoundingBox();

	bool Intersect(Ray r, IntersectPoint& p) override;

	
};