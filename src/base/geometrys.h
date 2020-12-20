#pragma once

#include "base/header.h"
#include "base/shape.h"


class Geometrys {

public:
	static Geometrys* Instance();

	std::vector<Shape*> shapes;


	bool Intersect(Ray r, FLOAT*t, int* index);

private:
	
	Geometrys();
};