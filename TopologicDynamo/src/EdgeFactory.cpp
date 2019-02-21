#include "EdgeFactory.h"

#include "Edge.h"

namespace Topologic
{
	namespace Factories
	{
		Topology ^ EdgeFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicCore::Edge::Ptr pCoreEdge = std::dynamic_pointer_cast<TopologicCore::Edge>(kpTopology.topologyPtr);
			if (pCoreEdge == nullptr)
			{
				return nullptr;
			}
			return gcnew Edge(pCoreEdge);
		}
	}
}