#pragma once


#include "base/header.h"
#include "base/bxdf.h"

class IntersectPoint {
public:

	FLOAT t;
	FLOAT weightU;
	FLOAT weightV;
	dVec3 normalWS;
	dVec3 tangentWS;
	dVec3 bitangentWS;
	dVec2 uv;

	struct {
		dVec3 normalWS;
		dVec3 tangentWS;
		dVec3 bitangentWS;
	}shading;
	
	//dVec2 uv;
	
	dMat3 tangentToWorld;
	dMat3 worldToTangent;

	int shapeIndex; //index of geometry::shape array
	int modelIndex;
	int meshIndex;
	int faceIndex;

	dVec3 Le(const dVec3& direction, const IntersectPoint& it);
    
    //for debug
    bool IsHitAreaLight();
    
    BSDF* bsdf;
    
    IntersectPoint(){bsdf = nullptr;}
    ~IntersectPoint(){if(bsdf)delete bsdf;}
    
    void ComputeScatteringFunctions(MeshInfo& meshInfo);
    
    BxDFType GetBxDFType(); 
};
