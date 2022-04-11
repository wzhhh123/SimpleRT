
#include "common.h"
#include "base/header.h"


int Bound2i::Area()
{
	return (max.x - min.x) * (max.y - min.y);
}

Point2i Bound2i::Extent()
{
	return { max.x - min.x, max.y - min.y };
}

