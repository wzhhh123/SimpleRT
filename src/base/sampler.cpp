
#include "sampler.h"


Sampler::~Sampler()
{

}

void Sampler::StartPixel(const Point2i& p)
{
	currentPixel = p;
	currentPixelSampleIndex = 0;
	array1DOffset = array2DOffset = 0;
}

bool Sampler::StartNextSample()
{
	array1DOffset = array2DOffset = 0;
	return ++currentPixelSampleIndex < samplesPerPixel;
}

Sampler::Sampler(int64_t samplesPerPixel): samplesPerPixel(samplesPerPixel)
{

}





bool GlobalSampler::StartNextSample()
{

}

void GlobalSampler::StartPixel(const Point2i& p)
{

}

FLOAT GlobalSampler::Get1D()
{

}

dVec2 GlobalSampler::Get2D()
{

}
