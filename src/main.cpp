

#include "base/datatoimage.h"
#include "base/renderer.h"
#include "raytracer/businesscard.h"
#include "raytracer/whitecolor.h"
#include "tool/imagehelper.h"
#include "base/geometrys.h"
#include "accelerate/linear.h"
#include "accelerate/bvh.h"
#include "raytracer/path.h"
#include "bxdf/lambert.h"
#include "base/sampling.h"



int main() {

	//for (int i = 0; i < 10; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		dVec2 p = { i / 10.0, j / 10.0 };
	//		dVec3 res = CosineSampleHemisphere(p);
	//		std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	//	}
	//}

	//Geometrys::Instance()->accelerater = new Linear();
	Geometrys::Instance()->accelerater = new Bvh();

	Renderer::Instance()->Initialize();
//	renderer.raytracer = new BusinessCard();
	//Renderer::Instance()->raytracer = new WhiteColor();
	Renderer::Instance()->raytracer = new Path();
	Renderer::Instance()->lambert = new Lambert();
	Renderer::Instance()->Run();
	Renderer::Instance()->deIntialize();

}