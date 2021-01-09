

#include "sampling.h"


//半球均匀采样
dVec3 UniformSampleHemisphere(const dVec2 &u) {
	FLOAT z = u[0];
	FLOAT r = std::sqrt(std::max((FLOAT)0, (FLOAT)1. - z * z));
	FLOAT phi = 2 * Pi * u[1];
	return dVec3(r * std::cos(phi), r * std::sin(phi), z);
}

FLOAT UniformHemispherePdf() { return Inv2Pi; } 


//同心圆采样圆盘
dVec2 ConcentricSampleDisk(const dVec2 &u) {
	// Map uniform random numbers to $[-1,1]^2$
	dVec2 uOffset = 2. * u - dVec2(1, 1);

	// Handle degeneracy at the origin
	if (uOffset.x == 0 && uOffset.y == 0) return dVec2(0, 0);

	// Apply concentric mapping to point
	FLOAT theta, r;
	if (std::abs(uOffset.x) > std::abs(uOffset.y)) {
		r = uOffset.x;
		theta = PiOver4 * (uOffset.y / uOffset.x);
	}
	else {
		r = uOffset.y;
		theta = PiOver2 - PiOver4 * (uOffset.x / uOffset.y);
	}
	return r * dVec2(std::cos(theta), std::sin(theta));
}


//cos加权的半球采样
dVec3 CosineSampleHemisphere(const dVec2 &u) {
	dVec2 d = ConcentricSampleDisk(u);
	FLOAT z = std::sqrt(std::max((FLOAT)0, 1 - d.x * d.x - d.y * d.y));
	return dVec3(d.x, d.y, z);

}

FLOAT CosineHemispherePdf(FLOAT cosTheta) { return cosTheta * InvPi; }

