#pragma once


#include "header.h"
#include "filter.h"
#include "header.h"
#include "struct/common.h"


struct TilePixel
{
    dVec3 weightedVal;
    FLOAT weightSum;
    
    TilePixel():weightedVal({0,0,0}), weightSum(0.0){}
};

class Film
{
public:
    
    Film(const Point2i& resolution, std::unique_ptr<Filter>filter, std::string filename);
    
    Bound2i GetSampleBound();
        
    std::unique_ptr<FilmTile> GetFilmTile(const Bound2i &sampleBound);

private:
    
    Point2i resolution;
    Bound2i sampleBound;
    
    std::string fileName;
    std::unique_ptr<float>imageData;
    std::unique_ptr<Filter>filter;
    
    
    struct Pixel
    {
        dVec3 rgb;
        float weightSum;
        Pixel()
        {
            rgb = dVec3{0,0,0};
            weightSum = 0;
        }
    };
    
    std::unique_ptr<Pixel>pixels;
    
};

class FilmTile
{
public:
    FilmTile(const Bound2i &tilePixelBound);
    
private:
    std::unique_ptr<TilePixel>tilePixels;
};
