

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

int main() {
	
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