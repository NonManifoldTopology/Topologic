#include <Bitwise.h>

namespace TopologicCore
{
	int Bitwise::And(const int kArgument1, const int kArgument2)
	{
		return kArgument1 & kArgument2;
	}

	int Bitwise::Or(const int kArgument1, const int kArgument2)
	{
		return kArgument1 | kArgument2;
	}

	int Bitwise::Xor(const int kArgument1, const int kArgument2)
	{
		return kArgument1 ^ kArgument2;
	}

	int Bitwise::Not(const int kArgument1)
	{
		return ~kArgument1;
	}

	unsigned int Bitwise::Not(const unsigned int kArgument1)
	{
		return ~kArgument1;
	}
}