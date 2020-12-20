

#include "base/datatoimage.h"
#include "base/renderer.h"
#include "raytracer/businesscard.h"
int main() {

	
	Renderer renderer;
	renderer.Initialize();
	renderer.raytracer = new BusinessCard();
	renderer.Run();
	renderer.deIntialize();

}