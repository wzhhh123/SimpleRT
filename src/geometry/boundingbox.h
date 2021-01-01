#pragma once
#include "base/shape.h"


//AABB BoundingBox
class BoundingBox : public Shape {

public:
	
	BoundingBox();

	bool Intersect(Ray r, IntersectPoint& p) override;

	void Intersect(BoundingBox& a);



	void Union(BoundingBox& a);

	void Union(dVec3& p);

	
	dVec3 amin, amax;
};




BoundingBox Union(const BoundingBox& a, const BoundingBox& b);
BoundingBox Union(const BoundingBox& a, const dVec3& b);

BoundingBox Intersect(const BoundingBox& a, const BoundingBox& b);

