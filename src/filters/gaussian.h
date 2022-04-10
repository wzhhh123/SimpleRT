#pragma once

#include "../base/header.h"
#include "../base/filter.h"

class GaussianFilter : public Filter
{
public:
    GaussianFilter(const dVec2& radius, FLOAT alpha)
        :Filter(radius),alpha(alpha),
        expX(std::exp(-alpha * radius.x * radius.x)),
        expY(std::exp(-alpha * radius.y * radius.y)){}
    FLOAT Evaluate(const dVec2 &p) const;
 
private:
    const FLOAT alpha;
    const FLOAT expX, expY;
    
    FLOAT Gaussian(FLOAT d,FLOAT expv) const
    {
        return std::max((FLOAT)0, FLOAT(std::exp(-alpha * d * d) - expv));
    }
};
