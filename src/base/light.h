#pragma once


#include "header.h"



class Light
{
public:
	
};


class AreaLight: public Light
{
public:
    AreaLight();
    virtual dVec3 L(const dVec3& direction, const IntersectPoint& it) const = 0;
};
