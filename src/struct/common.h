#pragma once


#include "base/header.h"


struct Bound2i
{
    Point2i min, max;
    Bound2i(){}
    Bound2i(Point2i minn, Point2i maxn):min(minn), max(maxn){}
    int Area();
    Point2i Extent();
};

struct Bound2f
{
    dVec2 min, max;
    Bound2f(){}
    Bound2f(dVec2 minn, dVec2 maxn):min(minn), max(maxn){}
    Bound2f(Bound2i boundi):min(dVec2(boundi.min.x, boundi.min.y)),max(dVec2(boundi.max.x, boundi.max.y)){}
};


