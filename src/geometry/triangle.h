#pragma once
#include "base/header.h"
#include "base/shape.h"
#include "struct/vertex.h"
#include "boundingbox.h"

class Triangle : public Shape {

public:
	
	Triangle(dVec3 _v0, dVec3 _v1, dVec3 _v2, dMat4 model, int _modelIndex, int _meshIndex, int _faceIndex);

	bool Intersect(Ray r, IntersectPoint& p) override;

	FLOAT Area() override;

	IntersectPoint Samping(dVec2 point, FLOAT* pdf);
	void SetData(IntersectPoint& it);

	Vertex v0;
	Vertex v1;
	Vertex v2;

	dVec3 tangent;
	dVec3 bitangent;

	dMat4 objectToWorld;

	BoundingBox boundingBox;

	int modelIndex;
	int meshIndex;
	int faceIndex;
	
	void GetUVs(dVec2 uv[3]);
	//void TangentToObject();


//private:
//	dMat3 tangentToObject;

};