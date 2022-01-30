#pragma once


#include "header.h"


enum BxDFType {
    BSDF_REFLECTION = 1 << 0,
    BSDF_TRANSMISSION = 1 << 1,
    BSDF_DIFFUSE = 1 << 2,
    BSDF_GLOSSY = 1 << 3,
    BSDF_SPECULAR = 1 << 4,
    BSDF_ALL = BSDF_DIFFUSE | BSDF_GLOSSY | BSDF_SPECULAR | BSDF_REFLECTION |
               BSDF_TRANSMISSION,
};


//tangent space
inline FLOAT CosTheta(const dVec3 &w) { return w.z; }
inline FLOAT Cos2Theta(const dVec3 &w) { return w.z * w.z; }
inline FLOAT AbsCosTheta(const dVec3 &w) { return std::abs(w.z); }
inline dVec3 FaceForward(dVec3 n, const dVec3 &v) { return (glm::dot(n, v) < 0.f) ? -n : n; }

float FrDielectric(float cosThetaI, float etaI, float etaT);

class BxDF {

public:
	
	virtual dVec3 F(const dVec3& wo, const dVec3& wi) = 0;

	virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
		const dVec2& sample, FLOAT* pdf);


	FLOAT Pdf(const dVec3 &wo, const dVec3 &wi);
    
    BxDF(BxDFType type):type(type){}
    
    virtual ~BxDF();
    
    
    BxDFType type;

};


class FresnelSpecular: public BxDF
{
public:
    
    FresnelSpecular(dVec3 R, dVec3 T, float etaA, float etaB);
    
    virtual dVec3 F(const dVec3& wo, const dVec3& wi);
    
    virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
        const dVec2& sample, FLOAT* pdf);
    
private:
    dVec3 R, T;
    float etaA, etaB;
    
};



class BSDF{
public:
  
    void Add(BxDF* b);
    virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
        const dVec2& sample, FLOAT* pdf);
    
    
    virtual dVec3 F(const dVec3& wo, const dVec3& wi);
    
    virtual ~BSDF();
        
private:
    BxDF* BxDFs[8];
    int NumBxDF = 0;
};



