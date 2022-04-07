
#include "sampler.h"



Sampler::~Sampler()
{

}

bool Sampler::StartNextSample()
{
	return false;
}

Sampler::Sampler(int64_t samplesPerPixel): samplesPerPixel(samplesPerPixel)
{

}

