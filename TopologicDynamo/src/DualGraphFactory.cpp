#include <DualGraphFactory.h>

#include <DualGraph.h>

namespace Topologic
{
	Topology ^ DualGraphFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
	{
		TopologicCore::DualGraph::Ptr pCoreDualGraph = std::dynamic_pointer_cast<TopologicCore::DualGraph>(kpTopology.topologyPtr);
		if (pCoreDualGraph == nullptr)
		{
			return nullptr;
		}
		return gcnew DualGraph_(pCoreDualGraph);
	}
}