#pragma once

#include "header.h"
#include "bxdf.h"



class Fresnel {
  public:
    // Fresnel Interface
    virtual ~Fresnel();
    virtual dVec3 Evaluate(FLOAT cosI) const = 0;
};


class FresnelConductor : public Fresnel {
  public:
    // FresnelConductor Public Methods
    dVec3 Evaluate(FLOAT cosThetaI) const;
    FresnelConductor(const dVec3 &etaI, const dVec3 &etaT,
                     const dVec3 &k)
        : etaI(etaI), etaT(etaT), k(k) {}

  private:
    dVec3 etaI, etaT, k;
};



class LambertianRefrection : public BxDF
{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& si);
	LambertianRefrection(dVec3 R, ImageTexture<dVec3, dVec3>* RTex) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_DIFFUSE)), R(R) , RTex(RTex){}

private:
	dVec3 R;
	ImageTexture<dVec3, dVec3>* RTex;
};

class FresnelNoOp : public Fresnel {
  public:
    dVec3 Evaluate(FLOAT) const { return dVec3(1,1,1); }
};

class SpecularReflection : public BxDF {
  public:
    // SpecularReflection Public Methods
    SpecularReflection(const dVec3 &R, Fresnel *fresnel)
        : BxDF(BxDFType(BSDF_REFLECTION | BSDF_SPECULAR)),
          R(R),
          fresnel(fresnel) {}

    virtual dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& is){return dVec3(0);}

    virtual dVec3 Sample_f(const dVec3& wo, dVec3* wi,
        const dVec2& sample, FLOAT* pdf, BxDFType& sampleType, IntersectPoint& is);    

  private:
    // SpecularReflection Private Data
    const dVec3 R;
    const Fresnel *fresnel;
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


