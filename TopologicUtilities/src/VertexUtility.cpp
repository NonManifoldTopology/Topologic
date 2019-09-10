#include "VertexUtility.h"

#include <TopologicCore/include/Vertex.h>

#include <algorithm>

namespace TopologicUtilities
{
	void VertexUtility::AdjacentEdges(
		const TopologicCore::Vertex::Ptr & kpVertex, 
		const TopologicCore::Topology::Ptr & kpParentTopology,
		std::list<TopologicCore::Edge::Ptr>& rCoreAdjacentEdges)
	{
		std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
		kpVertex->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Edge::Type(), coreAdjacentTopologies);
		for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
		{
			rCoreAdjacentEdges.push_back(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Edge>(kpAdjacentTopology)
			);
		}
	}
}