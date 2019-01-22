#include <Bitwise.h>

namespace TopologicCore
{
	int Bitwise::And(const std::list<int>& rArguments)
	{
		int result = 0;
		bool firstArgument = true;
		for (const int kArgument : rArguments)
		{
			if (firstArgument)
			{
				result = kArgument;
				firstArgument = false;
			}
			else
			{
				result &= kArgument;
			}
		}
		return result;
	}

	int Bitwise::Or(const std::list<int>& rArguments)
	{
		int result = 0;
		bool firstArgument = true;
		for (const int kArgument : rArguments)
		{
			if (firstArgument)
			{
				result = kArgument;
				firstArgument = false;
			}
			else
			{
				result |= kArgument;
			}
		}
		return result;
	}

	int Bitwise::Xor(const std::list<int>& rArguments)
	{
		int result = 0;
		bool firstArgument = true;
		for (const int kArgument : rArguments)
		{
			if (firstArgument)
			{
				result = kArgument;
				firstArgument = false;
			}
			else
			{
				result ^= kArgument;
			}
		}
		return result;
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