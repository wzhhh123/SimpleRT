#include "header.h"
#include "datatoimage.h"
#include "renderer.h"
#include "glm.hpp"
#include "ray.h"

void Renderer::Run()
{

	int yx = 0;
	while (yx < SIZE * SIZE) {

		glm::vec3 dir;
		dir.x = yx % SIZE - SIZE / 2;
		dir.z = SIZE / 2 - yx / SIZE;
		dir.y = SIZE / 2 / tan(AOV / 114.5915590261); // 360/PI~=114  计算近平面离相机距离

		Ray r = {};
		r.origin = glm::vec3{ 0,0,0 };
		r.direction = glm::normalize(dir);
		
		FLOAT t;
		if (s.Intersect(r, &t)) {
			imageData[yx] = glm::vec3{ 255,255,255 };
		}
		else{
			imageData[yx] = glm::vec3{ 0,0,0 };
		}
		++yx;
	}

	DataToImage(imageData);

}


void Renderer::Initialize() {

	imageData = new glm::vec3[SIZE*SIZE];

	s.center = glm::vec3{ 0., 10., 0 };
	s.radius = 1;

}

void Renderer::deIntialize() {

	delete[] imageData;

}