#pragma once

#include "base/header.h"
#include "base/light.h"



class DiffuseAreaLight: public AreaLight
{
public:
    DiffuseAreaLight(const dVec3 &Le,bool bTwoSized = false);
    
    virtual dVec3 L(const dVec3& direction, const IntersectPoint& it) const;

protected:
    bool bTwoSized;
    const dVec3 Lemit;
};



std::shared_ptr<AreaLight> CreateAreaLight(const dVec3 Le);
