#pragma once

#include "header.h"
#include "bxdf.h"



class LambertianRefrection : public BxDF
{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& si);
	LambertianRefrection(dVec3 R, ImageTexture<dVec3, dVec3>* RTex) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) , RTex(RTex){}

private:
	dVec3 R;
	ImageTexture<dVec3, dVec3>* RTex;
};



class FresnelSpecular : public BxDF
{
public:

	FresnelSpecular(dVec3 R, dVec3 T, float etaA, float etaB);

	virtual dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& is);

	virtual dVec3 Sample_f(const dVec3& wo, dVec3* wi,
		const dVec2& sample, FLOAT* pdf, BxDFType& sampleType, IntersectPoint& is);

private:
	dVec3 R, T;
	float etaA, etaB;

};


