#pragma once


#include "base/header.h"



class IntersectPoint {
public:

	FLOAT t;
	FLOAT weightU;
	FLOAT weightV;
	dVec3 normalWS;
	dVec3 normalOS;
	int shapeIndex; //index of geometry::shape array
	int modelIndex;
	int meshIndex;

	dVec3 Le(dVec3 direction);
};
