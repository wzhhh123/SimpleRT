#pragma once

#include "base/header.h"
#include "base/bxdf.h"
#include "base/material.h"

class LambertianRefrection : public BxDF{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi);
    LambertianRefrection(dVec3 albedo);

	dVec3 albedo;

};


class MatteMaterial: public Material{

public:
    
    MatteMaterial(const dVec3 Kd);
    virtual void ComputeScatteringFunctions(IntersectPoint& si) const;

    ~MatteMaterial();
        
private:
    dVec3 Kd;
    //LambertianRefrection* LambertRefractionPtr;
};


Material* CreateMatteMaterial(const dVec3 Kd);
