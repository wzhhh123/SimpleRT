
#include "imagehelper.h"


void SaveImage(int w, int h, int channels_num, unsigned char* imageData)
{
	//unsigned char *data = (unsigned char*)(new char[w * h * channels_num]);

	//int index = 0;
	//
	//for (int i = 0; i < h; ++i) {
	//	for (int j = 0; j < w; ++j) {
	//		for(int k = 0; k < channels_num; ++k)
	//			data[index] = imageData[index++];
	//	}
	//}

	stbi_write_jpg(OUTPUT_PATH_JPG, w, h, channels_num, imageData, w * channels_num);
}

//nori
dVec3 toSRGB(dVec3 col)  {
	dVec3 result;

	for (int i = 0; i < 3; ++i) {
		float value = col[i];

		if (value <= 0.0031308f)
			result[i] = 12.92f * value;
		else
			result[i] = (1.0f + 0.055f)
			* std::pow(value, 1.0f / 2.4f) - 0.055f;
	}

	return result;
}


dVec3 fastToneMap(dVec3 col) {

	return 1.0 / (col + 1.0);

}