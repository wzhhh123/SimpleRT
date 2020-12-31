
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

	stbi_write_jpg(OUTPUT_PATH, w, h, channels_num, imageData, w * channels_num);
}