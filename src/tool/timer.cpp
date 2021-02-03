#include "timer.h"
#include <iomanip>


std::string timeString(double time, bool precise) {
	if (std::isnan(time) || std::isinf(time))
		return "inf";
	std::string suffix = "ms";
	if (time > 1000) {
		time /= 1000; suffix = "s";
		if (time > 60) {
			time /= 60; suffix = "m";
			if (time > 60) {
				time /= 60; suffix = "h";
				if (time > 12) {
					time /= 12; suffix = "d";
				}
			}
		}
	}

	std::ostringstream os;
	os << std::setprecision(precise ? 4 : 1)
		<< std::fixed << time << suffix;

	return os.str();
}