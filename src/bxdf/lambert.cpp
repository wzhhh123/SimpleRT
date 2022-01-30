
#include "lambert.h"


LambertianRefrection::LambertianRefrection(dVec3 albedo) {
	this->albedo = albedo;
}


dVec3 LambertianRefrection::F(const dVec3& wo, const dVec3& wi) {
	return albedo * InvPi;
}

void MatteMaterial::ComputeScatteringFunctions(IntersectPoint& si) const
{
    si.bsdf = new BSDF();
    si.bsdf->Add(new LambertianRefrection(Kd));
}


MatteMaterial::MatteMaterial(const dVec3 Kd):Kd(Kd)
{
}

MatteMaterial::~MatteMaterial()
{

}

Material* CreateMatteMaterial(const dVec3 Kd)
{
    return new MatteMaterial(Kd);
}
