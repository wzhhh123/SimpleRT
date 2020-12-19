#pragma once

#include "header.h"
#include "geometry/sphere.h"



class Renderer {

public:
	
	void Initialize();
	void Run();
	void deIntialize();

	Sphere s;
	glm::vec3 *imageData;

};