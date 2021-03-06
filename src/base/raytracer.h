#pragma once
#include "header.h"
#include "glm.hpp"
#include "ray.h"

class RayTracer {
	
public:

	virtual dVec3 Trace(int level, Ray r) = 0;

};