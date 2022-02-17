#pragma once

#include "header.h"
#include "glm.hpp"

class Ray {

public:
	
	dVec3 origin;
	dVec3 direction;

};


class RayDifferential : public Ray
{
public:
	RayDifferential();

	bool bHasDifferentials;
	dVec3 rxOrigin, ryOrigin;
	dVec3 rxDirection, ryDirection;
};