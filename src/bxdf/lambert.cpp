
#include "lambert.h"


dVec3 Lambert::F(const dVec3& wo, const dVec3& wi) {
	//��/�У���С��1����ʾ�в��ֱ����գ�������ȫ����
	return albedo * InvPi;
}
