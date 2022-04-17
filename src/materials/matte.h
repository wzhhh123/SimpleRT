#pragma once

#include "base/header.h"
#include "base/bxdf.h"
#include "base/material.h"


class MatteMaterial: public Material{

public:
    
    MatteMaterial(const dVec3 Kd, ImageTexture<dVec3, dVec3>* tex);
    virtual void ComputeScatteringFunctions(IntersectPoint& si) const;

    ~MatteMaterial();
        
private:
    dVec3 Kd;
	ImageTexture<dVec3, dVec3>* tex;
    //LambertianRefrection* LambertRefractionPtr;
};


Material* CreateMatteMaterial(const dVec3 Kd, ImageTexture<dVec3, dVec3>* tex);
