#pragma once

#include "base/header.h"
#include "base/bxdf.h"
#include "base/material.h"

class GlassMaterial: public Material{

public:
    
    GlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index);
    virtual void ComputeScatteringFunctions(IntersectPoint& si) const;

private:
    dVec3 Kr,Kt;
    float Index;
};


Material* CreateGlassMaterial(const dVec3 Kr, const dVec3 Kt, const float Index);
