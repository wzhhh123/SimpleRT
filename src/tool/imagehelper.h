#pragma once

#include "base/header.h"
#include "stb_image_write.h"


void SaveImage(int w, int h, int channels_num, unsigned char* imageData);

dVec3 fastToneMap(dVec3 col);

float fastToneMap(float c);

dVec3 toSRGB(dVec3 col);

unsigned char* LoadImage(std::string filepath, int &w, int &h, int& bpp);

dVec3* LoadImageVec(std::string filepath, int &w, int &h);



