#include "About.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 7
#define BUILD_VERSION 1

namespace TopologicCore
{
	std::string About::Version()
	{
		return std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(BUILD_VERSION);
	}
}