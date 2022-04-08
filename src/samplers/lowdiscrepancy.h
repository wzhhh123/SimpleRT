#pragma once

#include "../base/header.h"
#include "pcg32.h"

FLOAT RadicalInverse(int baseIndex, uint64_t a);
static const int PrimeTableSize = 1000;
extern const int Primes[PrimeTableSize];
extern const int PrimeSums[PrimeTableSize];

std::vector<uint16_t> ComputeRadicalInversePermutations(pcg32& rng);


template<typename T>
void Shuffle(T* arr, int count, int Dims, pcg32& rng)
{
    for (int i = 0; i < count; ++i)
    {
        int other = i + rng.nextUInt(count - i);
        for (int j = 0; j < Dims; ++j)
        {
            std::swap(arr[Dims * i + j], arr[Dims * other + j]);
        }
    }
}


template<int base>
inline int64_t InverseRadicalInverse(uint64_t inverse, int nDigits)
{
    int64_t res = 0;
    while (nDigits--)
    {
        res = res * base + inverse % base;
        inverse /= base;
    }
    return res;
}


// Low Discrepancy Inline Functions
inline uint32_t ReverseBits32(uint32_t n) {
    n = (n << 16) | (n >> 16);
    n = ((n & 0x00ff00ff) << 8) | ((n & 0xff00ff00) >> 8);
    n = ((n & 0x0f0f0f0f) << 4) | ((n & 0xf0f0f0f0) >> 4);
    n = ((n & 0x33333333) << 2) | ((n & 0xcccccccc) >> 2);
    n = ((n & 0x55555555) << 1) | ((n & 0xaaaaaaaa) >> 1);
    return n;
}


inline uint64_t ReverseBits64(uint64_t n) {
    uint64_t n0 = ReverseBits32((uint32_t)n);
    uint64_t n1 = ReverseBits32((uint32_t)(n >> 32));
    return (n0 << 32) | n1;
}


FLOAT ScrambledRadicalInverse(int baseIndex, uint64_t a, const uint16_t* perm);