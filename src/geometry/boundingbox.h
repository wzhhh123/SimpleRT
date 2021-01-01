#pragma once
#include "base/shape.h"


//AABB BoundingBox
class BoundingBox : public Shape {

public:
	
	BoundingBox::BoundingBox();

	BoundingBox(dVec3 a, dVec3 b);

	bool Intersect(Ray r, IntersectPoint& p) override;

	void Intersect(BoundingBox& a);

	void Union(BoundingBox& a);

	void Union(dVec3& p);


	bool Cross(BoundingBox& box);

	int MaxExtent();

	dVec3 amin, amax;
};




BoundingBox Union(const BoundingBox& a, const BoundingBox& b);
BoundingBox Union(const BoundingBox& a, const dVec3& b);

BoundingBox Intersect(const BoundingBox& a, const BoundingBox& b);

