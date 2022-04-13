#include "halton.h"
#include "pcg32.h"
#include "lowdiscrepancy.h"
#include "../base/header.h"
#include "../struct/common.h"

static const int kMaxResolution = 128;

// HaltonSampler Utility Functions
static void extendedGCD(uint64_t a, uint64_t b, int64_t* x, int64_t* y);
static uint64_t multiplicativeInverse(int64_t a, int64_t n) {
    int64_t x, y;
    extendedGCD(a, n, &x, &y);
    return Mod(x, n);
}

static void extendedGCD(uint64_t a, uint64_t b, int64_t* x, int64_t* y) {
    if (b == 0) {
        *x = 1;
        *y = 0;
        return;
    }
    int64_t d = a / b, xp, yp;
    extendedGCD(b, a % b, &xp, &yp);
    *x = yp;
    *y = xp - (d * yp);
}


std::vector<uint16_t> HaltonSampler::radicalInversePermutations;

HaltonSampler::HaltonSampler(int samplesPerPixel, Bound2i sampleBound)
    :GlobalSampler(samplesPerPixel)
{
    if (radicalInversePermutations.empty())
    {
        pcg32 rng(8);
        radicalInversePermutations = ComputeRadicalInversePermutations(rng);
    }

    Point2i extent = sampleBound.max - sampleBound.min;
    for (int i = 0; i < 2; ++i)
    {
        int base = i ? 3 : 2;
        int scale = 1, exp = 0;
        while (scale < std::min(extent[i], kMaxResolution)) {
            scale *= base;
            ++exp;
        }
        baseScales[i] = scale;
        baseExponents[i] = exp;
    }

    sampleStride = baseScales[0] * baseScales[1];

    multInverse[0] = multiplicativeInverse(baseScales[1], baseScales[0]);
    multInverse[1] = multiplicativeInverse(baseScales[0], baseScales[1]);
}

int64_t HaltonSampler::GetIndexForSample(int64_t sampleNum) const
{
    if (currentPixel != pixelForOffset)
    {
        offsetForCurrentPixel = 0;
        if (sampleStride > 1)
        {
            Point2i pm(Mod(currentPixel[0], kMaxResolution),
                Mod(currentPixel[1], kMaxResolution));
            for (int i = 0; i < 2; ++i) {
                uint64_t dimOffset =
                    (i == 0)
                    ? InverseRadicalInverse<2>(pm[i], baseExponents[i])
                    : InverseRadicalInverse<3>(pm[i], baseExponents[i]);
                offsetForCurrentPixel +=
                    dimOffset * (sampleStride / baseScales[i]) * multInverse[i];
            }
            offsetForCurrentPixel %= sampleStride;
        }
    }
    return offsetForCurrentPixel + sampleNum * sampleStride;
}

FLOAT HaltonSampler::SampleDimension(int64_t index, int dim) const
{
    if (dim == 0)
        return RadicalInverse(dim, index >> baseExponents[0]);
    else if (dim == 1)
        return RadicalInverse(dim, index / baseScales[1]);
    else
        return ScrambledRadicalInverse(dim, index,
            PermutationForDimension(dim));
}


std::shared_ptr<Sampler> HaltonSampler::Clone()
{
    return std::shared_ptr<Sampler>(new HaltonSampler(*this));
}


