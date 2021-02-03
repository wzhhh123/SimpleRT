
#include "lambert.h"


Lambert::Lambert(dVec3 albedo) {
	this->albedo = albedo;
}


dVec3 Lambert::F(const dVec3& wo, const dVec3& wi) {
	//ρ/π，ρ小于1，表示有部分被吸收，不能完全反射
	return albedo * InvPi;
}
