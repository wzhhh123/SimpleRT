#pragma once

#include "header.h"
#include "ray.h"

class Camera
{
public:
	Camera();
	void GenerateRay(int Idx, dVec2 Sample, Ray& R);
    void GenerateRayDifferential(int Idx, dVec2& Samples, RayDifferential *Rd);
    
private:
	dVec3 dxCamera, dyCamera;
	float dirZ;
};
