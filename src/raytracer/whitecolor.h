#pragma once

#include "base/raytracer.h"


class WhiteColor : public RayTracer{

public:


	VEC3 Trace(int level, Ray r) override;


};