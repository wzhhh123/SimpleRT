#pragma once

#include "base/header.h"
#include "base/bxdf.h"
#include "base/material.h"



class FresnelSpecular : public BxDF
{
public:

	FresnelSpecular(dVec3 R, dVec3 T, float etaA, float etaB);

	virtual dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& is);

	virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
		const dVec2& sample, FLOAT* pdf, BxDFType& sampleType , IntersectPoint& is);

private:
	dVec3 R, T;
	float etaA, etaB;

};




class GlassMaterial: public Material{

public:
    
    GlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index);
    virtual void ComputeScatteringFunctions(IntersectPoint& si) const;

private:
    dVec3 Kr,Kt;
    float Index;
};


Material* CreateGlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index);
