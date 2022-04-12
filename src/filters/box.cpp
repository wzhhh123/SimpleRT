
#include "gaussian.h"
#include "box.h"


BoxFilter::BoxFilter(const dVec2& radius) :Filter(radius) 
{

}

FLOAT BoxFilter::Evaluate(const dVec2 &p) const
{
    return 1;
}















