#pragma once

#include "base/header.h"
#include "stb_image_write.h"


void SaveImage(int w, int h, int channels_num, unsigned char* imageData);

dVec3 fastToneMap(dVec3 col);

dVec3 toSRGB(dVec3 col);


