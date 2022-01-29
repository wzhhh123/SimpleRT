
#include "lambert.h"


LambertianRefrection::LambertianRefrection(dVec3 albedo) {
	this->albedo = albedo;
}


dVec3 LambertianRefrection::F(const dVec3& wo, const dVec3& wi) {
	return albedo * InvPi;
}
