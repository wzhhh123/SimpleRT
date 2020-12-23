#pragma once

#include "base/raytracer.h"
class BusinessCard  : public RayTracer{

public:


	dVec3 Trace(int level, Ray r) override;


};