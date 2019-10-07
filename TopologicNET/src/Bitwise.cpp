#include <Bitwise.h>

namespace Topologic
{
	namespace Utilities
	{
		int Bitwise::AND(List<int>^ arguments)
		{
			std::list<int> coreArguments;
			for each(int argument in arguments)
			{
				coreArguments.push_back(argument);
			}
			return TopologicUtilities::Bitwise::AND(coreArguments);
		}

		int Bitwise::OR(List<int>^ arguments)
		{
			std::list<int> coreArguments;
			for each(int argument in arguments)
			{
				coreArguments.push_back(argument);
			}
			return TopologicUtilities::Bitwise::OR(coreArguments);
		}

		int Bitwise::XOR(List<int>^ arguments)
		{
			std::list<int> coreArguments;
			for each(int argument in arguments)
			{
				coreArguments.push_back(argument);
			}
			return TopologicUtilities::Bitwise::XOR(coreArguments);
		}

		int Bitwise::NOT(int argument)
		{
			return TopologicUtilities::Bitwise::NOT(argument);
		}
	}
}