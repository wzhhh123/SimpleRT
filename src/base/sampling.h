#pragma once


#include "header.h"

dVec3 UniformSampleHemisphere(const dVec2 &u);

FLOAT UniformHemispherePdf();

dVec2 ConcentricSampleDisk(const dVec2 &u);

//cos加权的半球采样
dVec3 CosineSampleHemisphere(const dVec2 &u);

//cos加权半球采样的pdf
FLOAT CosineHemispherePdf(FLOAT cosTheta);




class Distribution1D {

public:

	Distribution1D(const FLOAT *f, int n);
	Distribution1D();

	int SampleDiscrete(FLOAT u, FLOAT* pdf) const;

	std::vector<FLOAT>func, cdf, pdf;
	FLOAT  totalCnt;
};






