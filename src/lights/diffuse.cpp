
#include "diffuse.h"
#include "struct/intersectpoint.h"


DiffuseAreaLight::DiffuseAreaLight(const dVec3 &Le,bool bTwoSized):Lemit(Le), bTwoSized(bTwoSized)
{
    Light();
}



dVec3 DiffuseAreaLight::L(const dVec3& direction, const IntersectPoint& it) const
{
    //return dVec3(0,0,0);
    return (bTwoSized||glm::dot(direction, it.shading.normalWS) > 0) ? Lemit : dVec3(0,0,0);
}

std::shared_ptr<AreaLight> CreateAreaLight(const dVec3 Le)
{
    return std::make_shared<DiffuseAreaLight>(Le);
}
