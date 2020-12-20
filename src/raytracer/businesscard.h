#pragma once

#include "base/raytracer.h"
class BusinessCard  : public RayTracer{

public:


	glm::dvec3 Trace(int level, Ray r) override;


};