#pragma once

#include "base/header.h"
#include "base/bxdf.h"
#include "base/material.h"

class LambertianRefrection : public BxDF{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi, IntersectPoint& si);
    LambertianRefrection(dVec3 albedo, ImageTexture<dVec3, dVec3>* tex);

	dVec3 albedo;
	ImageTexture<dVec3, dVec3>* tex;
};


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
