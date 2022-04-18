
#include "glass.h"
#include "base/reflection.h"




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
