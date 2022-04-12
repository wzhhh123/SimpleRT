#pragma once

#include "../base/header.h"
#include "../base/filter.h"

class BoxFilter: public Filter
{
public:
    BoxFilter(const dVec2& radius);
    FLOAT Evaluate(const dVec2& p) const;

private:

};
