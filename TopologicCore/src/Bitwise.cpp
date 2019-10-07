#include <Bitwise.h>

namespace TopologicUtilities
{
	int Bitwise::AND(const std::list<int>& rArguments)
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

	int Bitwise::OR(const std::list<int>& rArguments)
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

	int Bitwise::XOR(const std::list<int>& rArguments)
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

	int Bitwise::NOT(const int kArgument1)
	{
		return ~kArgument1;
	}

	unsigned int Bitwise::NOT(const unsigned int kArgument1)
	{
		return ~kArgument1;
	}
}