

#include "base/datatoimage.h"
#include "base/renderer.h"
#include "raytracer/businesscard.h"
#include "raytracer/whitecolor.h"
#include "tool/imagehelper.h"
int main() {
	

	Renderer renderer;
	renderer.Initialize();
//	renderer.raytracer = new BusinessCard();
	renderer.raytracer = new WhiteColor();
	renderer.Run();
	renderer.deIntialize();

}