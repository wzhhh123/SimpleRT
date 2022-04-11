
#include "film.h"
#include "header.h"
#include <cmath>
#include <algorithm>
#include <iostream>

Film::Film(const Point2i& resolution, std::unique_ptr<Filter>filt, std::string fileName):resolution(resolution), fileName(fileName), filter(std::move(filt))
{
    imageData = std::unique_ptr<float>(new float[CHANNEL_COUNT * resolution.x * resolution.y]);
    pixels = std::unique_ptr<Pixel>(new Pixel[resolution.x * resolution.y]);
    
    Point2i pMin = Point2i(std::floor(0.0+0.5f-(filter->radius.x)), std::floor(0.0+0.5f-(filter->radius.y)));
    Point2i pMax = Point2i(std::ceil(resolution.x-0.5+(filter->radius.x)), std::ceil(resolution.y-0.5+(filter->radius.y)));
    
    sampleBound = Bound2i(pMin, pMax);
    
}


Bound2i Film::GetSampleBound()
{
    return sampleBound;
}

std::unique_ptr<FilmTile>Film::GetFilmTile(const Bound2i &sampleBound)
{
    Point2f floatSampleBound = Point2f(sampleBound);
    dVec2 pMin = floatSampleBound.min + dVec2(0.5,0.5) - filter->radius;
    dvec2
    
}



FilmTile::FilmTile(const Bound2i &tilePixelBound)
{
    
}
