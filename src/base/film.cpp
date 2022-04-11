
#include "film.h"
#include "header.h"
#include <cmath>
#include <algorithm>
#include <iostream>
#include "renderer.h"

Film::Film(const Point2i& resolution, std::unique_ptr<Filter>filt, std::string fileName):resolution(resolution), fileName(fileName), filter(std::move(filt))
{
    imageData = std::unique_ptr<float>(new float[CHANNEL_COUNT * resolution.x * resolution.y]);
    pixels.resize(resolution.x * resolution.y);
    
    Point2i pMin = Point2i(std::floor(0.0+0.5f-(filter->radius.x)), std::floor(0.0+0.5f-(filter->radius.y)));
    Point2i pMax = Point2i(std::ceil(resolution.x-0.5+(filter->radius.x)), std::ceil(resolution.y-0.5+(filter->radius.y)));
    
    sampleBound = Bound2i(pMin, pMax);
    filePixelBound = Bound2i(Point2i(0,0), resolution);

    int offset = 0;
    for (int i = 0; i < filterTableWidth; ++i)
    {
        for (int j = 0; j < filterTableWidth; ++j, ++offset)
        {
            dVec2 sample = dVec2(((float)i + 0.5f) * filter->radius.x / filterTableWidth, ((float)j + 0.5f) * filter->radius.y / filterTableWidth);
            filterTable[offset] = filter->Evaluate(sample);
        }
    }

}

Film::Pixel& Film::GetPixel(Point2i p)
{
    return pixels[p.y * resolution.x + p.x];
}

Bound2i Film::GetSampleBound()
{
    return sampleBound;
}

std::unique_ptr<FilmTile>Film::GetFilmTile(const Bound2i &sampleBound)
{
    Bound2f floatSampleBound = Bound2f(sampleBound);
    dVec2 pMin = floatSampleBound.min + dVec2(0.5,0.5) - filter->radius;
    dVec2 pMax= floatSampleBound.max - dVec2(0.5, 0.5) + filter->radius;
    Bound2i tilePixelBound = Bound2i(floor(pMin), ceil(pMax));
    return std::unique_ptr<FilmTile>(new FilmTile(tilePixelBound, filePixelBound, filterTable));
}

void Film::MergeTile(std::shared_ptr<FilmTile> tile)
{
    std::lock_guard<std::mutex> lock(mutex);
    Bound2i tilePixelBound = tile->GetTilePixelBound();
    for (int i = tilePixelBound.min.x; i < tilePixelBound.max.x; ++i)
    {
        for (int j = tilePixelBound.min.y; j < tilePixelBound.max.y; ++j)
        {
            TilePixel& tilePixel = tile->GetPixel({ i,j });
            Pixel& filmPixel = GetPixel({ i,j });
            filmPixel.rgb += tilePixel.weightedVal;
            filmPixel.weightSum += tilePixel.weightSum;
        }
    }
}

void Film::SaveFilm()
{
    for (int i = 0; i < resolution.x; ++i)
    {
        for (int j = 0; j < resolution.y; ++j)
        {
            int index = i * resolution.x + j;
            Pixel& filmPixel = GetPixel({ j,i });
            filmPixel.rgb /= filmPixel.weightSum;
            Renderer::Instance()->imageData[index * CHANNEL_COUNT] = filmPixel.rgb.x;
            Renderer::Instance()->imageData[index * CHANNEL_COUNT + 1] = filmPixel.rgb.y;
            Renderer::Instance()->imageData[index * CHANNEL_COUNT + 2] = filmPixel.rgb.z;
        }
    }
}


FilmTile::FilmTile(const Bound2i& tileSampleBound, const Bound2i& filmPixelBound, FLOAT* filterTable)
{
    tilePixelBound = Bound2i(max(tileSampleBound.min, filmPixelBound.min), min(tileSampleBound.max, filmPixelBound.max));
    tilePixels.resize(tilePixelBound.Area());
    this->filterTable = filterTable;
}

TilePixel& FilmTile::GetPixel(Point2i p)
{
    int index = (p.y - tilePixelBound.min.y) * tilePixelBound.Extent().x + (p.x - tilePixelBound.min.x);
    assert(p.x >= tilePixelBound.min.x || p.x < tilePixelBound.max.x || p.y >= tilePixelBound.min.y || p.y < tilePixelBound.max.y);
    return tilePixels[index];
}


void FilmTile::AddSample(dVec2 p, dVec3 L)
{
    static const int filterTableSize = 16;
    static const float filterRadius = 2;
    int xMin = std::floor(-filterRadius + 0.5 + p.x);
    int xMax = std::floor(filterRadius + 0.5 + p.x);
    int yMin = std::floor(-filterRadius + 0.5 + p.y);
    int yMax = std::floor(filterRadius + 0.5 + p.y);

    for (int i = xMin; i < xMax; ++i)
    {
        for (int j = yMin; j < yMax; ++j)
        {
            if (i < tilePixelBound.min.x || i >= tilePixelBound.max.x || j < tilePixelBound.min.y || j >= tilePixelBound.max.y) continue;
            int x = abs(p.x - ((FLOAT)i + 0.5)) * filterTableSize / filterRadius;
            int y = abs(p.y - ((FLOAT)j + 0.5)) * filterTableSize / filterRadius;
            if (x == 16) x = 15;
            if (y == 16) y = 15;
            if(!(x >= 0 && x < filterTableSize && y >= 0 && y < filterTableSize))
                assert(x >= 0 && x < filterTableSize&& y >= 0 && y < filterTableSize);
            int filterIndex = x * filterTableSize + y;
            FLOAT weight = filterTable[filterIndex];
            TilePixel& pixel = GetPixel({ i,j });
            pixel.weightedVal += weight * L;
            pixel.weightSum += weight;
        }
    }

}

Bound2i FilmTile::GetTilePixelBound()
{
    return tilePixelBound;
}
