#pragma once

#include "header.h"
#include "struct/intersectpoint.h"

class Material{

public:

    virtual void ComputeScatteringFunctions(IntersectPoint& si) const = 0;
    virtual ~Material();

};
