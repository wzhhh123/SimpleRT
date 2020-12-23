#pragma once

#include "base/raytracer.h"


class WhiteColor : public RayTracer{

public:


	dVec3 Trace(int level, Ray r) override;


};