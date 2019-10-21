// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "VertexUtility.h"
#include "Edge.h"

#include <TopologicCore/include/Utilities/VertexUtility.h>

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