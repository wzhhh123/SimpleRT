

#include "raytracer.h"
#include "base/geometrys.h"
#include "pcg32.h"
#include "base/renderer.h"

dVec3 UniformSampleOneLight(pcg32& rng, IntersectPoint& point, Ray& r)
{
    FLOAT lightPdf = 0;
    int index = Geometrys::Instance()->lightDistribute.SampleDiscrete(rng.nextDouble(), &lightPdf);
    Triangle* triangle = dynamic_cast<Triangle*>(Geometrys::Instance()->shapes[Geometrys::Instance()->lightShapeIndices[index]]);
    FLOAT lightAreaPdf = 0;
    IntersectPoint it = triangle->Samping(dVec2{ rng.nextDouble(), rng.nextDouble() }, &lightAreaPdf);

    Ray shadowRay;
    shadowRay.origin = point.t * r.direction + r.origin;
    dVec3 lightPos = it.weightV * triangle->v0.vertexWS + it.weightU * triangle->v1.vertexWS + (1 - it.weightU - it.weightV) * triangle->v2.vertexWS;
    shadowRay.direction = glm::normalize(lightPos - shadowRay.origin);


    // Convert from area measure, as returned by the Sample() call
    // above, to solid angle measure.
    lightAreaPdf *= glm::distance(lightPos, shadowRay.origin) / abs(glm::dot(glm::normalize(it.normalWS), -shadowRay.direction));

    IntersectPoint nearestHit;
    bool found = Geometrys::Instance()->Intersect(shadowRay, &nearestHit);
    if (found) {
        dVec3 col = nearestHit.Le(-shadowRay.direction, nearestHit);

        dVec3 f = Renderer::Instance()->GetBxDF(point.modelIndex, point.meshIndex)->F(shadowRay.direction, shadowRay.direction);
        return col * f * std::abs(glm::dot(shadowRay.direction, point.normalWS)) / (lightPdf * lightAreaPdf);
        return col;// / (lightPdf * lightAreaPdf);
    }
    else {
        return dVec3(0, 0, 0);
    }
}
