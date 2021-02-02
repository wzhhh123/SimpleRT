

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


Distribution1D::Distribution1D(const FLOAT *f, int n) : func(f, f + n), cdf(n + 1), pdf(n) {

	cdf[0] = 0;

	totalCnt = 0;
	for (int i = 0; i < func.size(); ++i) {
		totalCnt += func[i];
	}

	cdf[0] = 0;
	for (int i = 1; i <= n; ++i) {
		cdf[i] = cdf[i - 1] + func[i - 1] / totalCnt;
	}

	for (int i = 0; i < n; ++i) pdf[i] = func[i] / totalCnt;

}


Distribution1D::Distribution1D(){}



int Distribution1D::SampleDiscrete(FLOAT u, FLOAT* pdf) const {

	int offset = 0;

	for (int i = 0; i < cdf.size(); ++i) {
		if (cdf[i] <= u) {
			offset = i;
			break;
		}
	}

	*pdf = this->pdf[offset];
	return offset;
}
