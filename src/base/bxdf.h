#pragma once


#include "header.h"



class BxDF {

public:
	
	//����brdf  ������ܲ���delta�ֲ��ģ���Ҫ������һ��Sample_F
	virtual dVec3 F(const dVec3& wo, const dVec3& wi) = 0;

};