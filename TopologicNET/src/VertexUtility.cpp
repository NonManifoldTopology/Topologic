// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

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