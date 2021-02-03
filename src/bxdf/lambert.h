#pragma once

#include "base/header.h"
#include "base/bxdf.h"

class Lambert : public BxDF{

public:
	dVec3 F(const dVec3& wo, const dVec3& wi);
	Lambert(dVec3 albedo);

	dVec3 albedo;

};
