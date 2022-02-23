
#include "glass.h"


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



FresnelSpecular::FresnelSpecular(dVec3 R, dVec3 T, float etaA, float etaB) : BxDF(BxDFType(BSDF_REFLECTION | BSDF_TRANSMISSION | BSDF_SPECULAR)), R(R), T(T), etaA(etaA), etaB(etaB)
{

}


//delta distrubution, 	aribitrary wi have no scattering, zero is returned.
dVec3 FresnelSpecular::F(const dVec3& wo, const dVec3& wi, IntersectPoint& is)
{
	return dVec3(0, 0, 0);
}


dVec3 FresnelSpecular::Sample_f(const dVec3 &wo, dVec3* wi,
	const dVec2& sample, FLOAT* pdf, BxDFType& sampleType, IntersectPoint& is)
{
	FLOAT Fr = FrDielectric(CosTheta(wo), etaA, etaB);
	if (Fr > sample.x)
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

		if (!Refract(wo, FaceForward(dVec3(0, 0, 1), wo), etaI / etaT, wi))
		{
			return dVec3(0, 0, 0);
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



void GlassMaterial::ComputeScatteringFunctions(IntersectPoint& si) const
{
    si.bsdf = new BSDF();
    si.bsdf->Add(new FresnelSpecular(Kr, Kt, 1, Index));
}

GlassMaterial::GlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index):Kr(Kr), Kt(Kt), Index(Index){}


Material* CreateGlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index)
{
    return new GlassMaterial(Kr, Kt, Index);
}
