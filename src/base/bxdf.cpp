

#include "bxdf.h"
#include "sampling.h"



inline FLOAT AbsCosTheta(const dVec3 &w) { return std::abs(w.z); }


inline bool SameHemisphere(const dVec3 &w, const dVec3 &wp) {
	return w.z * wp.z > 0;
}


FLOAT BxDF::Pdf(const dVec3 &wo, const dVec3 &wi)
{
	return SameHemisphere(wo, wi) ? CosineHemispherePdf(AbsCosTheta(wi)) : 0;
}


dVec3 BxDF::Sample_f(const dVec3 &wo, dVec3* wi,
	const dVec2& sample, FLOAT* pdf)  {

	*wi = CosineSampleHemisphere(sample);
	if (wo.z < 0)wi->z *= -1;
	*pdf = Pdf(wo, *wi);
	return F(wo, *wi);
}


BxDF::~BxDF()
{
    
}


void BSDF::Add(BxDF* b)
{
    BxDFs[NumBxDF++] = b;
}


dVec3 BSDF::Sample_f(const dVec3 &wo, dVec3* wi,
    const dVec2& sample, FLOAT* pdf)
{
    return BxDFs[0]->Sample_f(wo, wi, sample, pdf);
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
