

#include "linear.h"
#include "base/shape.h"


void Linear::Initialize(std::vector<Shape*>*scene)
{
	this->scene = scene;
}

bool Linear::Intersect(Ray& r, IntersectPoint* p, int* index)
{
	bool found = false;
	FLOAT mint = 1e30;
	IntersectPoint nearestHit{};
	for (size_t i = 0; i < scene->size(); ++i)
	{
		IntersectPoint p;
		Shape* shape = (*scene)[i];
		if (shape->Intersect(r, p)) {
			if (!found) {
				found = true;
				nearestHit = p;
				mint = p.t;
				*index = i;
			}
			else if (mint > p.t) {
				mint = p.t;
				*index = i;
				nearestHit = p;
			}
		}
	}
	*p = nearestHit;
	return found;

}
