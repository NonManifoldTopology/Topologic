#include <GraphFactory.h>

#include <Graph.h>

namespace Topologic
{
	Topology ^ GraphFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
	{
		TopologicExtension::Graph::Ptr pCoreGraph = std::dynamic_pointer_cast<TopologicExtension::Graph>(kpTopology.topologyPtr);
		if (pCoreGraph == nullptr)
		{
			return nullptr;
		}
		return gcnew Extension::Graph(pCoreGraph);
	}
}