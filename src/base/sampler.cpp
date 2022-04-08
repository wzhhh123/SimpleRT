
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

bool Sampler::SetSampleNumber(int64_t sampleNum)
{
    array1DOffset = array2DOffset = 0;
    currentPixelSampleIndex = sampleNum;
    return currentPixelSampleIndex < samplesPerPixel;
}

bool GlobalSampler::StartNextSample()
{
    return false;
}

void GlobalSampler::StartPixel(const Point2i& p)
{

}

FLOAT GlobalSampler::Get1D()
{
    return† SampleDimension(intervalSampleIndex, dimension++);
}

dVec2 GlobalSampler::Get2D()
{
    dVec2 p{SampleDimension(intervalSampleIndex, dimension),SampleDimension(intervalSampleIndex, dimension+1)};
    dimension += 2;
    return p;
}

bool GlobalSampler::SetSampleNumber(int64_t sampleNum)
{
    dimension = 0;
    intervalSampleIndex = GetIndexForSample(sampleNum);
    return Sampler::SetSampleNumber(<#sampleNum#>);
}
