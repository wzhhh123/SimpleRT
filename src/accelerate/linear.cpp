

#include "linear.h"
#include "base/shape.h"
#include "geometry/triangle.h"

void Linear::Initialize(std::vector<Shape*>*scene)
{
	this->scene = scene;
}

bool Linear::Intersect(Ray& r, IntersectPoint* p)
{
	bool found = false;
	FLOAT mint = 1e30;
	IntersectPoint nearestHit{};
	for (size_t i = 0; i < scene->size(); ++i)
	{
		IntersectPoint p;
		Shape* shape = (*scene)[i];
		Triangle* tri = (Triangle*)shape;
		//if (tri->boundingBox.Intersect(r, p)) {
		if (shape->Intersect(r, p)) {
			if (!found) {
				found = true;
				mint = p.t;
				nearestHit = p;
				nearestHit.index = i;
			}
			else if (mint > p.t) {
				mint = p.t;
				nearestHit = p;
				nearestHit.index = i;
			}
		}
	}
	*p = nearestHit;
	return found;

}
