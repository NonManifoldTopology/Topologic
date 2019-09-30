#include "VertexUtility.h"
#include "Edge.h"

#include <TopologicUtilities/include/VertexUtility.h>

namespace Topologic {
	namespace Utilities {
		List<Edge^>^ VertexUtility::AdjacentEdges(Vertex ^ vertex, Topology ^ parentTopology)
		{
			TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Edge::Ptr> coreAdjacentEdges;
			try {
				TopologicUtilities::VertexUtility::AdjacentEdges(pCoreVertex, pCoreParentTopology, coreAdjacentEdges);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}

			List<Edge^>^ adjacentEdges = gcnew List<Edge^>();
			for (std::list<TopologicCore::Edge::Ptr>::const_iterator kAdjacentEdgeIterator = coreAdjacentEdges.begin();
				kAdjacentEdgeIterator != coreAdjacentEdges.end();
				kAdjacentEdgeIterator++)
			{
				Edge^ edge = gcnew Edge(*kAdjacentEdgeIterator);
				adjacentEdges->Add(edge);
			}

			return adjacentEdges;
		}

		double VertexUtility::Distance(Vertex ^ vertex, Topology ^ topology)
		{
			TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

			return TopologicUtilities::VertexUtility::Distance(pCoreVertex, pCoreTopology);
		}

	}
}