#include <Bitwise.h>

namespace Topologic
{
	namespace Utilities
	{
		int Bitwise::And(List<int>^ arguments)
		{
			std::list<int> coreArguments;
			for each(int argument in arguments)
			{
				coreArguments.push_back(argument);
			}
			return TopologicCore::Bitwise::And(coreArguments);
		}

		int Bitwise::Or(List<int>^ arguments)
		{
			std::list<int> coreArguments;
			for each(int argument in arguments)
			{
				coreArguments.push_back(argument);
			}
			return TopologicCore::Bitwise::Or(coreArguments);
		}

		int Bitwise::Xor(List<int>^ arguments)
		{
			std::list<int> coreArguments;
			for each(int argument in arguments)
			{
				coreArguments.push_back(argument);
			}
			return TopologicCore::Bitwise::Xor(coreArguments);
		}

		int Bitwise::Not(int argument)
		{
			return TopologicCore::Bitwise::Not(argument);
		}
	}
}