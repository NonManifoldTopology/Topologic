#pragma once

#include "Utilities.h"

namespace TopologicUtility
{
	class Bitwise
	{
	public:
		static TOPOLOGIC_SUPPORT_API int And(const int kArgument1, const int kArgument2);

		static TOPOLOGIC_SUPPORT_API int Or(const int kArgument1, const int kArgument2);

		static TOPOLOGIC_SUPPORT_API int Xor(const int kArgument1, const int kArgument2);

		static TOPOLOGIC_SUPPORT_API int Not(const int kArgument1);

		static TOPOLOGIC_SUPPORT_API unsigned int Not(const unsigned int kArgument1);
	};
}