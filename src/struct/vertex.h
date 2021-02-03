#pragma once


#include "base/header.h"


class Vertex{

public:


	dVec2 uv;

	dVec3 vertexWS;
	dVec3 normalOS;
	dVec3 normalWS;
	dVec3 tangentWS;
	dVec3 tangentOS;
	dVec3 bitangentWS;
	dVec3 bitangentOS;

	dMat4 objectToWorld;

	Vertex(dVec3 _v, dVec3 _n, dVec2 _uv, dVec3 _t, dVec3 _bt, dMat4 _objectToWorld, int _modelIndex);

	Vertex();

//void SetTangentToObject(dMat3 _tangentToObject);

private:
	dMat3 tangentToObject;
	dVec3 v;

	int modelIndex;

};