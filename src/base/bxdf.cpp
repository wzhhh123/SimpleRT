

#include "bxdf.h"
#include "sampling.h"
#include "struct/intersectpoint.h"



inline bool SameHemisphere(const dVec3 &w, const dVec3 &wp) {
	return w.z * wp.z > 0;
}


FLOAT BxDF::Pdf(const dVec3 &wo, const dVec3 &wi)
{
	return SameHemisphere(wo, wi) ? CosineHemispherePdf(AbsCosTheta(wi)) : 0;
}


dVec3 BxDF::Sample_f(const dVec3 &wo, dVec3* wi,
	const dVec2& sample, FLOAT* pdf, BxDFType& type, IntersectPoint& is)  {

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


int BSDF::NumComponents(const BxDFType &type)
{
    int num = 0;
    for (int i = 0; i < NumBxDF; ++i)
    {
        if (BxDFs[i]->MatchFlag(type))
        {
            ++num;
        }
    }
    return num;
}

dVec3 BSDF::Sample_f(const dVec3& woW, dVec3* wiW,
    const dVec2& sample, FLOAT* pdf, BxDFType type, IntersectPoint& is, BxDFType flag, BxDFType* sampledType)
{
    int matchComps = NumComponents(flag);
    if (matchComps == 0)
    {
        *pdf = 0;
        return dVec3(0);
    }
    int comp = std::min(matchComps - 1, (int)std::floor(sample.x * matchComps));

    BxDF* bxdf = nullptr;
    for (int i = 0; i < NumBxDF; ++i, --comp)
    {
        if (BxDFs[i]->MatchFlag(flag) && comp == 0)
        {
            bxdf = BxDFs[i];
            break;
        }
    }

    dVec2 remapSample = { std::min(sample[0] * matchComps - comp, OneMinusEpsilon),
                      sample[1] };

    dVec3 wi, wo = is.worldToTangent * woW;
    if (wo.z == 0) return dVec3(0);
    *pdf = 0;
    if (sampledType) *sampledType = bxdf->type;
    dVec3 f = bxdf->Sample_f(wo, &wi, remapSample, pdf, type, is);
    if (*pdf == 0)
    {
        if (sampledType) *sampledType = BxDFType(0);
        return dVec3(0);
    }
    *wiW = is.tangentToWorld * wi;
    if (!(bxdf->type & BSDF_SPECULAR) && matchComps > 1)
    {
        for (int i = 0; i < NumBxDF; ++i)
        {
            if (bxdf != BxDFs[i] && BxDFs[i]->MatchFlag(flag))
            {
                *pdf += BxDFs[i]->Pdf(wo, wi);
            }
        }
    }
    if (matchComps > 1) *pdf /= matchComps;
    if (!(bxdf->type & BSDF_SPECULAR))
    {
        bool reflect = glm::dot(*wiW, is.shading.normalWS) * glm::dot(woW, is.shading.normalWS) > 0;
        f = dVec3(0);
        for (int i = 0; i < NumBxDF; ++i)
            if (BxDFs[i]->MatchFlag(type) &&
                ((reflect && (BxDFs[i]->type & BSDF_REFLECTION)) ||
                    (!reflect && (BxDFs[i]->type & BSDF_TRANSMISSION))))
                f += BxDFs[i]->F(wo, wi, is);
    }

    return f;
    //return BxDFs[0]->Sample_f(wo, wi, sample, pdf, type, is);
}


dVec3 BSDF::F(const dVec3& woW, const dVec3& wiW, IntersectPoint& is, BxDFType flags)
{
    dVec3 wo = is.worldToTangent * woW, wi = is.worldToTangent * wiW;
    if (wo.z == 0) return dVec3(0);
    //both negative or both positive will result in a positive bool value.
    bool reflect = glm::dot(wiW, is.shading.normalWS) * glm::dot(woW, is.shading.normalWS) > 0;

    dVec3 f(0);
    for (int i = 0; i < NumBxDF; ++i)
    {
        //every bxdf flag has either bsdf_reflection or bsdf_transimission.
        if (BxDFs[i]->MatchFlag(flags) && (reflect && (BxDFs[i]->type & BSDF_REFLECTION)) || (!reflect && (BxDFs[i]->type & BSDF_TRANSMISSION)))
        {
            f += BxDFs[i]->F(wo, wi, is);
        }
    }

    return f;
    //return BxDFs[0]->F(wo,wi, is);
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
