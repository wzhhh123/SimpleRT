#pragma once

#include "base/header.h"
#include "base/bxdf.h"

class Lambert : public BxDF{

public:

	dVec3 F(const dVec3& wo, const dVec3& wi);
	dVec3 Sample_F(const dVec3& wo, const dVec3& wi);



	dVec3 albedo;

};
