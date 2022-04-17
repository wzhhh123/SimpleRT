#include "reflection.h"
#include "header.h"
#include "texture.h"



dVec3 LambertianRefrection::F(const dVec3& wo, const dVec3& wi, IntersectPoint& si)
{
	if (RTex)
	{
		return RTex->Evaluate(si) * InvPi;
	}
	return R * InvPi;
}

