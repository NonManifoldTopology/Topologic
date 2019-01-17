#include <GraphFactory.h>

#include <Graph.h>

namespace Topologic
{
	Topology ^ GraphFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
	{
		TopologicExtensions::DualGraph_::Ptr pCoreGraph = std::dynamic_pointer_cast<TopologicExtensions::DualGraph_>(kpTopology.topologyPtr);
		if (pCoreGraph == nullptr)
		{
			return nullptr;
		}
		return gcnew Extensions::DualGraph_(pCoreGraph);
	}
}