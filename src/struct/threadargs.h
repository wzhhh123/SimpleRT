#pragma once


#include "base/header.h"





struct TileRenderThreadInput
{
	int left;
	int right;
};


struct TileRenderThreadOutput
{
	int pixelCnt;
	unsigned char* imageData;
};
