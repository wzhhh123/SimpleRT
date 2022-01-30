
#include "glass.h"



void GlassMaterial::ComputeScatteringFunctions(IntersectPoint& si) const
{
    
}

GlassMaterial::GlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index):Kr(Kr), Kt(Kt), Index(Index){}


GlassMaterial* CreateGlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index)
{
    return new GlassMaterial(Kr, Kt, Index);
}
