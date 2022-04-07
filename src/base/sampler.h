#pragma once


#include "header.h"




class Sampler
{
public:
	virtual ~Sampler();

	virtual FLOAT Get1D() = 0;
	virtual dVec2 Get2D() = 0;

	Sampler(int64_t samplesPerPixel);

	virtual bool StartNextSample();

	const int64_t samplesPerPixel;
};