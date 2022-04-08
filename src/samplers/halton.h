#pragma once

#include "../base/header.h"
#include "base/sampler.h"
#include "lowdiscrepancy.h"

class HaltonSampler : public GlobalSampler
{
public:
    
    HaltonSampler(int nSample, Point2i pMin, Point2i pMax);
    
    int64_t GetIndexForSample(int64_t sampleNum) const;
    
    FLOAT SampleDimension(int64_t index, int dimension) const;
    

private:
    static std::vector<uint16_t> radicalInversePermutations;
    Point2i baseScales, baseExponents;
    int sampleStride;
    int multInverse[2];

    mutable Point2i pixelForOffset = Point2i(std::numeric_limits<int>::max(),
        std::numeric_limits<int>::max());
    mutable int64_t offsetForCurrentPixel;

    const uint16_t* PermutationForDimension(int dim) const {
        if (dim >= PrimeTableSize)
        return &radicalInversePermutations[PrimeSums[dim]];
    }
};
