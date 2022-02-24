
#include "imagehelper.h"
#include "stb_image.h"
#include "exrhelper.h"

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
dVec3 toSRGB(dVec3 col) 
{
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


dVec3 fastToneMap(dVec3 col) 
{
	return 1.0 / (col + 1.0);
}


unsigned char* LoadImage(std::string filepath, int &w, int &h, int &bpp)
{
    stbi_set_flip_vertically_on_load(true);
    unsigned char* pixels = stbi_load(filepath.c_str(), &w, &h, &bpp, 0);
    return pixels;
}


dVec3* LoadImageVec(std::string filepath, int &w, int &h)
{
    int bpp;
	unsigned char* pixels = LoadImage(filepath, w, h, bpp);
    assert(bpp >= 3);
	dVec3* res = new dVec3[w*h];
	for(int i = 0; i < h; ++i)
	{
		for(int j = 0; j < w; ++j)
		{
            res[i*w + j] = dVec3{ pixels[(i*w + j)*bpp], pixels[(i*w + j)*bpp + 1], pixels[(i*w + j)*bpp + 2] };
		}
	}
    //float* pixels2 = new float[w*h*3];
    //for(int i = 0; i < h; ++i)
    //{
    //    for(int j = 0; j < w; ++j)
    //    {
    //        pixels2[i*w*3+j*3] = pixels[i*w*4+j*4] / 255.0;
    //        pixels2[i*w*3+j*3+1] = pixels[i*w*4+j*4+1] / 255.0;
    //        pixels2[i*w*3+j*3+2] = pixels[i*w*4+j*4+2] / 255.0;
    //    }
    //}
    //EXR_HELPER::SaveAsExrFile("../../assets/scenes/testtex.exr", w,h, pixels2);
    
    std::cout << res[0].x <<" " << res[0].y <<" " << res[0].z << std::endl;
	free(pixels);
	return res;
}
