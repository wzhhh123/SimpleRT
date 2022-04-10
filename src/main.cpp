

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
#include "filters/gaussian.h"

#include "cstdio"
#include "cstdlib"
/*
build log:
1��tbb��libҪ����Ϊtbb_debug��Ȼ���tbbd.dll���Ƶ�debug����
2��assimpҪbuild shared
*/

/*
ϵͳ��Ϣ:
1����������ϵ
*/


void FIlterTest()
{
	using namespace std;

	GaussianFilter* filter = new GaussianFilter({ 1,1 }, 1);

	FLOAT totalVal = 0;
	for (int i = 0; i <= 30; ++i) {
		for (int j = 0; j <= 30; ++j) {
			dVec2 p = { (i - 15) / 7.5f, (j - 15) / 7.5f };
			FLOAT val = (filter->Evaluate(p));
			totalVal += val;
			printf("%f ", val);
		}
		cout << endl;
	}
	printf("%f ", totalVal);

}

int main() {
	using namespace std;
	//BoundingBox aabb = BoundingBox({ -1,-1,-1 }, { 1,1,1 });
	//Ray r;
	//r.origin = { 0,0,0 };
	//r.direction = { 0,0,1 };
	//float t0, t1;
	//aabb.IntersectTest(r, t0, t1);
	//cout << t0 << " " << t1 << endl;
	//return 0;

	//for (int i = 0; i < 10; ++i) {
	//	for (int j = 0; j < 10; ++j) {
	//		dVec2 p = { i / 10.0, j / 10.0 };
	//		dVec3 res = CosineSampleHemisphere(p);
	//		std::cout << res.x << " " << res.y << " " << res.z << std::endl;
	//	}
	//}

	//FIlterTest();
	//return 0;
	// 
	//Geometrys::Instance()->accelerater = new Linear();
	Geometrys::Instance()->accelerater = new Bvh();

	Renderer::Instance()->Initialize();
//	renderer.raytracer = new BusinessCard();
	//Renderer::Instance()->raytracer = new WhiteColor();
	Renderer::Instance()->raytracer = new Path();
	Renderer::Instance()->Run();
	//Renderer::Instance()->RunHaltonSample();
	Renderer::Instance()->deIntialize();

}
