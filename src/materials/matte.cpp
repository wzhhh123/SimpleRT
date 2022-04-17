
#include "matte.h"
#include "base/texture.h"
#include "base/reflection.h"



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
