#pragma once

#include "Utilities.h"

#include <list>

namespace TopologicUtilities
{
	class Bitwise
	{
	public:
		static TOPOLOGIC_API int AND(const std::list<int>& rArguments);

		static TOPOLOGIC_API int OR(const std::list<int>& rArguments);

		static TOPOLOGIC_API int XOR(const std::list<int>& rArguments);

		static TOPOLOGIC_API int NOT(const int kArgument1);

		static TOPOLOGIC_API unsigned int NOT(const unsigned int kArgument1);
	};
}