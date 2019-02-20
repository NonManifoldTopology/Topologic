#include "About.h"

#include <TopologicCore/include/About.h>

namespace Topologic
{
	String ^ Topologic::About::Version()
	{
		std::string coreVersion = TopologicCore::About::Version();
		return gcnew String(coreVersion.c_str());
	}
}