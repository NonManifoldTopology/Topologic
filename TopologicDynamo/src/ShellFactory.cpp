#include "ShellFactory.h"
#include "Shell.h"

namespace Topologic
{
	namespace Factories
	{
		Topology ^ ShellFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicCore::Shell::Ptr pCoreShell = std::dynamic_pointer_cast<TopologicCore::Shell>(kpTopology.topologyPtr);
			if (pCoreShell == nullptr)
			{
				return nullptr;
			}
			return gcnew Shell(pCoreShell);
		}
	}
}