#pragma once


#include "header.h"
#include "filter.h"
#include "header.h"
#include "struct/common.h"
#include <thread>
#include <mutex>

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

    void MergeTile(std::shared_ptr<FilmTile> tile);

    void SaveFilm();

private:
    
    Point2i resolution;
    Bound2i sampleBound;
    Bound2i filePixelBound;

    std::string fileName;
    std::unique_ptr<float>imageData;
    std::unique_ptr<Filter>filter;
    
    static const int filterTableWidth = 16;
    FLOAT filterTable[filterTableWidth * filterTableWidth];


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

    Pixel& GetPixel(Point2i p);

    std::vector<Pixel>pixels;
    std::mutex mutex;
};

class FilmTile
{
public:
    FilmTile(const Bound2i &tileSampleBound, const Bound2i &filmPixelBound, FLOAT* filterTable, int filterTableSize, const dVec2& filterRadius);
    
    TilePixel& GetPixel(Point2i p);

    void AddSample(dVec2 p, dVec3 L);

    Bound2i GetTilePixelBound();

private:
    std::vector<TilePixel>tilePixels;
    Bound2i tilePixelBound;
    FLOAT* filterTable;

    int filterTableSize;
    dVec2 filterRadius;
};
