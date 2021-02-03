
#include "lambert.h"


Lambert::Lambert(dVec3 albedo) {
	this->albedo = albedo;
}


dVec3 Lambert::F(const dVec3& wo, const dVec3& wi) {
	//��/�У���С��1����ʾ�в��ֱ����գ�������ȫ����
	return albedo * InvPi;
}
