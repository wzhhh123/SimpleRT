

#include "bxdf.h"
#include "sampling.h"




inline bool SameHemisphere(const dVec3 &w, const dVec3 &wp) {
	return w.z * wp.z > 0;
}


FLOAT BxDF::Pdf(const dVec3 &wo, const dVec3 &wi)
{
	return SameHemisphere(wo, wi) ? CosineHemispherePdf(AbsCosTheta(wi)) : 0;
}


dVec3 BxDF::Sample_f(const dVec3 &wo, dVec3* wi,
	const dVec2& sample, FLOAT* pdf, BxDFType& type, IntersectPoint& is)  {

    type = (BxDFType)(BSDF_DIFFUSE | BSDF_REFLECTION);
	*wi = CosineSampleHemisphere(sample);
	if (wo.z < 0)wi->z *= -1;
	*pdf = Pdf(wo, *wi);
	return F(wo, *wi, is);
}


BxDF::~BxDF()
{
    
}



void BSDF::Add(BxDF* b)
{
    BxDFs[NumBxDF++] = b;
}


dVec3 BSDF::Sample_f(const dVec3 &wo, dVec3* wi,
    const dVec2& sample, FLOAT* pdf, BxDFType& type, IntersectPoint& is)
{
    return BxDFs[0]->Sample_f(wo, wi, sample, pdf, type, is);
}


dVec3 BSDF::F(const dVec3& wo, const dVec3& wi, IntersectPoint& is)
{
    return BxDFs[0]->F(wo,wi, is);
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
