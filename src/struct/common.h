#pragma once


#include "base/header.h"


struct Bound2i
{
    Point2i min, max;
    Bound2i(){}
    Bound2i(Point2i minn, Point2i maxn):min(minn), max(maxn){}
};

struct Bound2f
{
    dVec2 min, max;
    Bound2f(){}
    Bound2f(dVec2 minn, dVec2 maxn):min(minn), max(maxn){}
    Bound2f(Bound2i boundi):min({boundi.min.x, boundi.min.y}),max({boundi.max.x, boundi.max.y}){}
};


