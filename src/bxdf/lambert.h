#pragma once

#include "base/header.h"
#include "base/bxdf.h"

class LambertianRefrection : public BxDF{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi);
    LambertianRefrection(dVec3 albedo);

	dVec3 albedo;

};
