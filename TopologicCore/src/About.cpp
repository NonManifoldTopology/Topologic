#include "About.h"

#define MAJOR_VERSION 0
#define MINOR_VERSION 8
#define EXTERNAL_BUILD_VERSION 0
#define INTERNAL_BUILD_VERSION 1

namespace TopologicCore
{
	std::string About::Version()
	{
		return std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(EXTERNAL_BUILD_VERSION) + "." + std::to_string(INTERNAL_BUILD_VERSION);
	}
}