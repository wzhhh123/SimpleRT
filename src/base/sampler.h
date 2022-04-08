#pragma once


#include "header.h"




class Sampler
{
public:
	virtual ~Sampler();

	virtual FLOAT Get1D() = 0;
	virtual dVec2 Get2D() = 0;

	Sampler(int64_t samplesPerPixel);

	virtual void StartPixel(const Point2i& p);

	virtual bool StartNextSample();
    
    virtual bool SetSampleNumber(int64_t sampleNum);

	const int64_t samplesPerPixel;
	Point2i currentPixel;
	int64_t currentPixelSampleIndex;

	int array1DOffset, array2DOffset;
};



class GlobalSampler : public Sampler
{
public:
	bool StartNextSample();
    bool SetSampleNumber(int64_t sampleNum);
	void StartPixel(const Point2i& p);
	FLOAT Get1D();
	dVec2 Get2D();

	GlobalSampler(int64_t samplesPerPixel) : Sampler(samplesPerPixel) {}
	virtual int64_t GetIndexForSample(int64_t sampleNum) const = 0;
	virtual FLOAT SampleDimension(int64_t index, int dimension) const = 0;

private:
	int dimension;
	int64_t intervalSampleIndex;
	static const int arrayStartDim = 5;
	int arrayEndDim;
};
