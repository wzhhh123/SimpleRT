#pragma once


#include "header.h"


class Filter
{
public:
    virtual ~Filter();
    Filter(const dVec2 &radius):radius(radius), invRadius(1.0/radius){}
    virtual FLOAT Evaluate(const dVec2 &p) const = 0;
    
    const dVec2 radius, invRadius;
};
