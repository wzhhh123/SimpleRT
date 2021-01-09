#pragma once


#include "header.h"

dVec3 UniformSampleHemisphere(const dVec2 &u);


FLOAT UniformHemispherePdf();


dVec2 ConcentricSampleDisk(const dVec2 &u);

//cos��Ȩ�İ������
dVec3 CosineSampleHemisphere(const dVec2 &u);

//cos��Ȩ���������pdf
FLOAT CosineHemispherePdf(FLOAT cosTheta);
