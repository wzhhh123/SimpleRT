

#include "geometrys.h"


Geometrys* Geometrys::Instance() {

	static Geometrys *geometrys = new Geometrys();
	return geometrys;

}


bool Geometrys::Intersect(Ray r, IntersectPoint* p, int* index)
{
	bool found = false;
	FLOAT mint = 1e30;
	IntersectPoint nearestHit{};
	for (size_t i = 0; i < shapes.size(); ++i)
	{
		IntersectPoint p;
		Shape* shape = shapes[i];
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

Geometrys::Geometrys() {}


