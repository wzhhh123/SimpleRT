
#include "gaussian.h"

FLOAT GaussianFilter::Evaluate(const dVec2 &p) const
{
    return Gaussian(p.x, expX) * Gaussian(p.y, expY);
}















