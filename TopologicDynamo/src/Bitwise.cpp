#include <Bitwise.h>

namespace Topologic
{
	namespace Utility
	{
		int Bitwise::And(int argument1, int argument2)
		{
			return TopologicUtility::Bitwise::And(argument1, argument2);
		}

		int Bitwise::Or(int argument1, int argument2)
		{
			return TopologicUtility::Bitwise::Or(argument1, argument2);
		}

		int Bitwise::Xor(int argument1, int argument2)
		{
			return TopologicUtility::Bitwise::And(argument1, argument2);
		}

		int Bitwise::Not(int argument)
		{
			return TopologicUtility::Bitwise::Not(argument);
		}
	}
}