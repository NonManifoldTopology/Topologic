#pragma once

#include "Utilities.h"

#include <list>
namespace TopologicCore
{
	class Bitwise
	{
	public:
		static TOPOLOGIC_API int And(const std::list<int>& rArguments);

		static TOPOLOGIC_API int Or(const std::list<int>& rArguments);

		static TOPOLOGIC_API int Xor(const std::list<int>& rArguments);

		static TOPOLOGIC_API int Not(const int kArgument1);

		static TOPOLOGIC_API unsigned int Not(const unsigned int kArgument1);
	};
}