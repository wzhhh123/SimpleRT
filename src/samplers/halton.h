#pragma once

#include "../base/header.h"




class HaltonSampler : public GlobalSampler
{
public:
    
    HaltonSampler(int nSample, Point2i pMin, Point2i pMax);
    
    int64_t GetIndexForSample(int64_t sampleNum) const;
    
    FLOAT SampleDimension(int64_t index, int dimension) const;
    
};
