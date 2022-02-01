

#include "bxdf.h"
#include "sampling.h"



float FrDielectric(float cosThetaI, float etaI, float etaT) {
    cosThetaI = clamp(cosThetaI, -1, 1);
    // Potentially swap indices of refraction
    bool entering = cosThetaI > 0.f;
    if (!entering) {
        std::swap(etaI, etaT);
        cosThetaI = std::abs(cosThetaI);
    }

    // Compute _cosThetaT_ using Snell's law
    float sinThetaI = std::sqrt(std::max((float)0, 1 - cosThetaI * cosThetaI));
    float sinThetaT = etaI / etaT * sinThetaI;

    // Handle total internal reflection
    if (sinThetaT >= 1) return 1;
    float cosThetaT = std::sqrt(std::max((float)0, 1 - sinThetaT * sinThetaT));
    float Rparl = ((etaT * cosThetaI) - (etaI * cosThetaT)) /
                  ((etaT * cosThetaI) + (etaI * cosThetaT));
    float Rperp = ((etaI * cosThetaI) - (etaT * cosThetaT)) /
                  ((etaI * cosThetaI) + (etaT * cosThetaT));
    return (Rparl * Rparl + Rperp * Rperp) / 2;
}

// purely deduction of snell law
// https://pbr-book.org/3ed-2018/Reflection_Models/Specular_Reflection_and_Transmission#eq:transmitted-radiance-change
inline bool Refract(const dVec3 &wi, const dVec3 &n, FLOAT eta,
                    dVec3 *wt) {
    // Compute $\cos \theta_\roman{t}$ using Snell's law
    FLOAT cosThetaI = glm::dot(n, wi);
    FLOAT sin2ThetaI = std::max(FLOAT(0), FLOAT(1 - cosThetaI * cosThetaI));
    FLOAT sin2ThetaT = eta * eta * sin2ThetaI;
    
    // Handle total internal reflection for transmission
    // internal reflection accur when sin2thetaT is equal or grater than 1
    if (sin2ThetaT >= 1)
    {
        return false;
    }
    FLOAT cosThetaT = std::sqrt(1 - sin2ThetaT);
    *wt = eta * -wi + (eta * cosThetaI - cosThetaT) * dVec3(n);
    return true;
}

inline bool SameHemisphere(const dVec3 &w, const dVec3 &wp) {
	return w.z * wp.z > 0;
}


FLOAT BxDF::Pdf(const dVec3 &wo, const dVec3 &wi)
{
	return SameHemisphere(wo, wi) ? CosineHemispherePdf(AbsCosTheta(wi)) : 0;
}


dVec3 BxDF::Sample_f(const dVec3 &wo, dVec3* wi,
	const dVec2& sample, FLOAT* pdf, BxDFType& type)  {

    type = (BxDFType)(BSDF_DIFFUSE | BSDF_REFLECTION);
	*wi = CosineSampleHemisphere(sample);
	if (wo.z < 0)wi->z *= -1;
	*pdf = Pdf(wo, *wi);
	return F(wo, *wi);
}


BxDF::~BxDF()
{
    
}


FresnelSpecular::FresnelSpecular(dVec3 R, dVec3 T, float etaA, float etaB): BxDF(BxDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)), R(R), T(T), etaA(etaA), etaB(etaB)
{
  
}


//delta distrubution, 	aribitrary wi have no scattering, zero is returned.
dVec3 FresnelSpecular::F(const dVec3& wo, const dVec3& wi)
{
    return dVec3(0,0,0);
}


dVec3 FresnelSpecular::Sample_f(const dVec3 &wo, dVec3* wi,
    const dVec2& sample, FLOAT* pdf, BxDFType& sampleType)
{
    FLOAT Fr = FrDielectric(CosTheta(wo), etaA, etaB);
     if(Fr > sample.x)
    {
        //specular refrection term
        *wi = dVec3(-wo.x, -wo.y, wo.z);
        // if (sampledType)
        sampleType = BxDFType(BSDF_SPECULAR | BSDF_REFLECTION);
        *pdf = Fr;
        // the expect rendering equation of delta distribution is Lo = Fr*Li when wi is equal to the refrection direction of w0,
        // the native rendering equation is L0 = Fr*Li*costheta, so we should divide costheta here for the correct purpose.
        return R * Fr / AbsCosTheta(*wi);
    }
    else
    {
        //specular transimission
        bool entering = CosTheta(wo) > 0;
        float etaI = entering ? etaA : etaB;
        float etaT = entering ? etaB : etaA;
        
        if(!Refract(wo, FaceForward(dVec3(0,0,1), wo), etaI/etaT, wi))
        {
            return dVec3(0,0,0);
        }
        
        dVec3 ft = T * (1 - Fr);

        // Account for non-symmetry with transmission to different medium
        //if (mode == TransportMode::Radiance)
            ft *= (etaI * etaI) / (etaT * etaT);
        //if (sampledType)
        sampleType = BxDFType(BSDF_SPECULAR | BSDF_TRANSMISSION);
        *pdf = 1 - Fr;
        //also divide costheta here to get the expected rendering equation of delta distribution

        return ft / AbsCosTheta(*wi);
    }
}


void BSDF::Add(BxDF* b)
{
    BxDFs[NumBxDF++] = b;
}


dVec3 BSDF::Sample_f(const dVec3 &wo, dVec3* wi,
    const dVec2& sample, FLOAT* pdf, BxDFType& type)
{
    return BxDFs[0]->Sample_f(wo, wi, sample, pdf, type);
}


dVec3 BSDF::F(const dVec3& wo, const dVec3& wi)
{
    return BxDFs[0]->F(wo,wi);
}


BSDF::~BSDF()
{
    for(int i = 0; i < NumBxDF; ++i)
    {
        delete BxDFs[i];
    }
}


BxDFType BSDF::GetBxDFType()
{
    int type = 0;
    for(int i = 0; i < NumBxDF; ++i)
    {
        type |= BxDFs[i]->type;
    }
    return (BxDFType)type;
}
