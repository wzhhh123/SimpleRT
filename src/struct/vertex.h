#pragma once


#include "base/header.h"


class Vertex{

public:

	dVec3 vertexWS;
	dMat4 objectToWorld;
	Vertex(dVec3 _v, dMat4 _objectToWorld, int _modelIndex);
	Vertex();

private:
	dMat3 tangentToObject;
	dVec3 v;
	int modelIndex;

};