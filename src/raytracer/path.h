#pragma once

#include "base/raytracer.h"
#include "pcg32.h"
#include "struct/intersectpoint.h"


class Path : public RayTracer{

public:
	dVec3 Trace(int level, Ray r) override;
	dVec3 UniformSampleOneLight(pcg32& rng, IntersectPoint& point, Ray &r);

};