#pragma once


#include "base/header.h"



class IntersectPoint {
public:

	FLOAT t;
	FLOAT weightU;
	FLOAT weightV;
	dVec3 normalWS;
	dVec3 normalOS;
	dVec2 uv;
	
	dMat3 tangentToWorld;

	int shapeIndex; //index of geometry::shape array
	int modelIndex;
	int meshIndex;

	dVec3 Le(dVec3 direction);
};
