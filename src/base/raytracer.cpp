

#include "raytracer.h"
#include "base/geometrys.h"
#include "pcg32.h"
#include "base/renderer.h"
#include "base/sampler.h"

dVec3 UniformSampleOneLight(std::shared_ptr<Sampler>& sampler, IntersectPoint& point, Ray& r)
{
    dVec3 L = dVec3(0,0,0);
    FLOAT lightPdf = 0;
    int index = Geometrys::Instance()->lightDistribute.SampleDiscrete(sampler->Get1D(), &lightPdf);
    Triangle* triangle = dynamic_cast<Triangle*>(Geometrys::Instance()->shapes[Geometrys::Instance()->lightShapeIndices[index]]);
    FLOAT lightAreaPdf = 0;
    IntersectPoint it = triangle->Samping(sampler->Get2D(), &lightAreaPdf);

    Ray shadowRay;
    shadowRay.origin = point.t * r.direction + r.origin;
    dVec3 lightPos = it.weightV * triangle->v0.vertexWS + it.weightU * triangle->v1.vertexWS + (1 - it.weightU - it.weightV) * triangle->v2.vertexWS;
    shadowRay.direction = glm::normalize(lightPos - shadowRay.origin);

    //importance sampling for lights
    {
        // Convert from area measure, as returned by the Sample() call
        // above, to solid angle measure.
        FLOAT lightAreaToSolidAnglePdf = lightAreaPdf * glm::distance(lightPos, shadowRay.origin) / abs(glm::dot(glm::normalize(it.shading.normalWS), -shadowRay.direction));
        IntersectPoint nearestHit;
        bool found = Geometrys::Instance()->Intersect(shadowRay, &nearestHit);
        if (found)
        {
            dVec3 col = nearestHit.Le(-shadowRay.direction, nearestHit);
            
            //dVec3 f = Renderer::Instance()->GetBxDF(point.modelIndex, point.meshIndex)->F(shadowRay.direction, shadowRay.direction);
            dVec3 f = point.bsdf->F(shadowRay.direction, shadowRay.direction, nearestHit);
            //if (col.r < 1e-6 && col.g < 1e-6 && col.g < 1e-6)
            //{
            //    L = { 100,0,0 };
            //}
            L += col * f * std::abs(glm::dot(shadowRay.direction, point.shading.normalWS)) / (lightAreaToSolidAnglePdf);
        }
        else
        {
            L += dVec3(0, 0, 0);
        }
    }
    
    
    //importance sampling for brdf
    {
        
    }
    
    
    return L / lightPdf;
}
