#include "DualGraphFactory.h"
#include "DualGraph.h"

namespace Topologic
{
	namespace Factories
	{
		Topology ^ DualGraphFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicExtensions::DualGraph_::Ptr pCoreGraph = std::dynamic_pointer_cast<TopologicExtensions::DualGraph_>(kpTopology.topologyPtr);
			if (pCoreGraph == nullptr)
			{
				return nullptr;
			}
			return gcnew Extensions::DualGraph_(pCoreGraph);
		}
	}
}