#include "platform.h"



// convert ../../asset to ../asset
std::string ConvertRelativePath(std::string rawpath)
{
#if __APPLE__

#else
	rawpath = rawpath.substr(rawpath.find_first_of('/') + 1);
#endif 
	return rawpath;
}
