#pragma once

#include "base/header.h"
#include "base/bxdf.h"
#include "base/material.h"


class MirrorMaterial: public Material{

public:
    
    MirrorMaterial(const dVec3 Kr, ImageTexture<dVec3, dVec3>* bunmMap):Kr(Kr), bumpMap(bumpMap){}
    virtual void ComputeScatteringFunctions(IntersectPoint& si) const;

    ~MirrorMaterial();
        
private:
    dVec3 Kr;
	ImageTexture<dVec3, dVec3>* bumpMap;
    //LambertianRefrection* LambertRefractionPtr;
};


Material* CreateMirrorMaterial(const dVec3 Kr, ImageTexture<dVec3, dVec3>* bumpMap);
