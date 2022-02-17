#pragma once

#include "header.h"

class Camera
{
public:
	Camera();
	void GenerateRay(int Idx, dVec2 Sample, Ray& R);

private:
	dVec3 dxCamera, dyCamera;
	float dirZ;
};