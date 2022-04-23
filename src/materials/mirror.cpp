
#include "mirror.h"
#include "base/texture.h"
#include "base/reflection.h"



void MirrorMaterial::ComputeScatteringFunctions(IntersectPoint& si) const
{
    si.bsdf = new BSDF();
    si.bsdf->Add(new SpecularReflection(Kr, new FresnelNoOp()));
}

MirrorMaterial::~MirrorMaterial(){}

Material* CreateMirrorMaterial(const dVec3 Kr, ImageTexture<dVec3, dVec3>* bumpMap)
{
    return new MirrorMaterial(Kr, bumpMap);
}

