#pragma once


#include "header.h"



class BxDF {

public:
	
	//返回brdf  这个不能采样delta分布的，需要额外来一个Sample_F
	virtual dVec3 F(const dVec3& wo, const dVec3& wi) = 0;

	virtual dVec3 Sample_f(const dVec3 &wo, dVec3* wi,
		const dVec2& sample, FLOAT* pdf);


	FLOAT Pdf(const dVec3 &wo, const dVec3 &wi);


};