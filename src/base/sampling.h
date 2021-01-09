#pragma once


#include "header.h"

dVec3 UniformSampleHemisphere(const dVec2 &u);


FLOAT UniformHemispherePdf();


dVec2 ConcentricSampleDisk(const dVec2 &u);

//cos加权的半球采样
dVec3 CosineSampleHemisphere(const dVec2 &u);

//cos加权半球采样的pdf
FLOAT CosineHemispherePdf(FLOAT cosTheta);
