#pragma once

#include "base/raytracer.h"


class Path : public RayTracer{

public:


	dVec3 Trace(int level, Ray r) override;


};