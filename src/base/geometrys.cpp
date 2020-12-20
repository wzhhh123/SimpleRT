

#include "geometrys.h"


Geometrys* Geometrys::Instance() {

	static Geometrys *geometrys = new Geometrys();
	return geometrys;

}


bool Geometrys::Intersect(Ray r, FLOAT* t, int* index)
{
	bool found = false;
	FLOAT mint = 1e30;
	for (size_t i = 0; i < shapes.size(); ++i)
	{
		FLOAT t;
		Shape* shape = shapes[i];
		if (shape->Intersect(r, &t)) {
			if (!found) {
				found = true;
				mint = t;
				*index = i;
			}
			else if (mint > t) {
				mint = t;
				*index = i;
			}
		}
	}
	*t = mint;
	return found;
}

Geometrys::Geometrys() {}


