#pragma once
#include "header.h"
#include "glm.hpp"
#include "ray.h"
#include "pcg32.h"

class RayTracer {
	
public:
	virtual dVec3 Trace(int level, Ray r, std::shared_ptr<Sampler>sampler) = 0;

};


dVec3 UniformSampleOneLight(std::shared_ptr<Sampler>&sampler, IntersectPoint& point, Ray &r);
