#pragma once

#include "header.h"
#include "bxdf.h"



class LambertianRefrection : public BxDF{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& si);
	LambertianRefrection(dVec3 R, ImageTexture<dVec3, dVec3>* RTex) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) , RTex(RTex){}

private:
	dVec3 R;
	ImageTexture<dVec3, dVec3>* RTex;
};

