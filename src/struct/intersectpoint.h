#pragma once


#include "base/header.h"



class IntersectPoint {
public:

	FLOAT t;
	FLOAT weightU;
	FLOAT weightV;
	dVec3 normalWS;
	//dVec3 normalOS;
	dVec3 tangentWS;
	dVec3 bitangentWS;
	
	//dVec2 uv;
	
	dMat3 tangentToWorld;
	dMat3 worldToTangent;

	int shapeIndex; //index of geometry::shape array
	int modelIndex;
	int meshIndex;
	int faceIndex;

	dVec3 Le(const dVec3& direction, const IntersectPoint& it);
};
