#pragma once
#include "header.h"
#include "ray.h"
#include "struct/intersectpoint.h"



class Shape {

public:
	
	virtual bool Intersect(Ray r, IntersectPoint& p) = 0;

	virtual FLOAT Area();

};