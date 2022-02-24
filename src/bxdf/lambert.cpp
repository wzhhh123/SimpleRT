
#include "lambert.h"
#include "base/texture.h"

LambertianRefrection::LambertianRefrection(dVec3 albedo, ImageTexture<dVec3, dVec3>* tex): BxDF((BxDFType)(BSDF_DIFFUSE)){
	this->albedo = albedo;
	this->tex = tex;
}


dVec3 LambertianRefrection::F(const dVec3& wo, const dVec3& wi, IntersectPoint& si) {
	if(tex)
	{
		return tex->Evaluate(si)*InvPi;
	}
	return albedo * InvPi;
}

void MatteMaterial::ComputeScatteringFunctions(IntersectPoint& si) const
{
    si.bsdf = new BSDF();
    si.bsdf->Add(new LambertianRefrection(Kd, tex));
}


MatteMaterial::MatteMaterial(const dVec3 Kd, ImageTexture<dVec3, dVec3> *tex):Kd(Kd), tex(tex)
{
}

MatteMaterial::~MatteMaterial()
{

}

Material* CreateMatteMaterial(const dVec3 Kd, ImageTexture<dVec3, dVec3>* tex)
{
    return new MatteMaterial(Kd, tex);
}
