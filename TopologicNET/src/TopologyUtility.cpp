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

#include "TopologyUtility.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "Cell.h"

#include <TopologicCore/include/Utilities/TopologyUtility.h>

namespace Topologic
{
	namespace Utilities
	{
		Topology^ TopologyUtility::Translate(Topology^ topology, double x, double y, double z)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

			TopologicCore::Topology::Ptr pCoreTranslatedTopology = TopologicUtilities::TopologyUtility::Translate(pCoreTopology, x, y, z);

			return Topology::ByCoreTopology(pCoreTranslatedTopology);
		}

		Topology ^ TopologyUtility::Rotate(Topology ^ topology, 
			Vertex^ origin,
			double xVector, double yVector, double zVector,
			double degree)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreOrigin =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(origin->GetCoreTopologicalQuery());

			TopologicCore::Topology::Ptr pCoreRotatedTopology = TopologicUtilities::TopologyUtility::Rotate(pCoreTopology, pCoreOrigin, xVector, yVector, zVector, degree);

			return Topology::ByCoreTopology(pCoreRotatedTopology);
		}

		Topology ^ TopologyUtility::Scale(Topology ^ topology, Vertex ^ origin, double xFactor, double yFactor, double zFactor)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreOrigin =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(origin->GetCoreTopologicalQuery());

			TopologicCore::Topology::Ptr pCoreScaledTopology = TopologicUtilities::TopologyUtility::Scale(pCoreTopology, pCoreOrigin, xFactor, yFactor, zFactor);

			return Topology::ByCoreTopology(pCoreScaledTopology);
		}

		Topology ^ TopologyUtility::Transform(Topology ^ topology,
			double translationX, double translationY, double translationZ,
			double rotation11, double rotation12, double rotation13,
			double rotation21, double rotation22, double rotation23,
			double rotation31, double rotation32, double rotation33)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();

			TopologicCore::Topology::Ptr pCoreTransformedTopology =
				TopologicUtilities::TopologyUtility::Transform(
					pCoreTopology, 
					translationX, translationY, translationZ, 
					rotation11, rotation12, rotation13,
					rotation21, rotation22, rotation23,
					rotation31, rotation32, rotation33);

			return Topology::ByCoreTopology(pCoreTransformedTopology);
		}

		List<Topology^>^ TopologyUtility::AdjacentTopologies(Topology ^ topology, Topology ^ parentTopology, int typeFilter)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
			try {
				TopologicUtilities::TopologyUtility::AdjacentTopologies(pCoreTopology, pCoreParentTopology, typeFilter, coreAdjacentTopologies);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}

			List<Topology^>^ adjacentTopologies = gcnew List<Topology^>();
			for (std::list<TopologicCore::Topology::Ptr>::const_iterator kAdjacentTopologyIterator = coreAdjacentTopologies.begin();
				kAdjacentTopologyIterator != coreAdjacentTopologies.end();
				kAdjacentTopologyIterator++)
			{
				Topology^ topology = Topology::ByCoreTopology(*kAdjacentTopologyIterator);
				adjacentTopologies->Add(topology);
			}

			return adjacentTopologies;
		}

		/*List<Edge^>^ TopologyUtility::AdjacentEdges(Topology ^ topology, Topology ^ parentTopology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Edge::Ptr> coreAdjacentEdges;
			TopologicUtilities::TopologyUtility::AdjacentEdges(pCoreTopology, pCoreParentTopology, coreAdjacentEdges);

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

		List<Face^>^ TopologyUtility::AdjacentFaces(Topology ^ topology, Topology ^ parentTopology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Face::Ptr> coreAdjacentFaces;
			TopologicUtilities::TopologyUtility::AdjacentFaces(pCoreTopology, pCoreParentTopology, coreAdjacentFaces);

			List<Face^>^ adjacentFaces = gcnew List<Face^>();
			for (std::list<TopologicCore::Face::Ptr>::const_iterator kAdjacentFaceIterator = coreAdjacentFaces.begin();
				kAdjacentFaceIterator != coreAdjacentFaces.end();
				kAdjacentFaceIterator++)
			{
				Face^ Face = gcnew Topologic::Face(*kAdjacentFaceIterator);
				adjacentFaces->Add(Face);
			}

			return adjacentFaces;
		}

		List<Cell^>^ TopologyUtility::AdjacentCells(Topology ^ topology, Topology ^ parentTopology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Cell::Ptr> coreAdjacentCells;
			TopologicUtilities::TopologyUtility::AdjacentCells(pCoreTopology, pCoreParentTopology, coreAdjacentCells);

			List<Cell^>^ adjacentCells = gcnew List<Cell^>();
			for (std::list<TopologicCore::Cell::Ptr>::const_iterator kAdjacentCellIterator = coreAdjacentCells.begin();
				kAdjacentCellIterator != coreAdjacentCells.end();
				kAdjacentCellIterator++)
			{
				Cell^ Cell = gcnew Topologic::Cell(*kAdjacentCellIterator);
				adjacentCells->Add(Cell);
			}

			return adjacentCells;
		}*/

		/*Topology ^ TopologyUtility::Transform(Topology ^ topology,
			double zAxisX, double zAxisY, double zAxisZ,
			double xAxisX, double xAxisY, double xAxisZ)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();

			TopologicCore::Topology::Ptr pCoreTransformedTopology =
					TopologicUtilities::TopologyUtility::Transform(
						pCoreTopology, pCoreCenterOfMass,
						zAxisX, zAxisY, zAxisZ,
						xAxisX, xAxisY, xAxisZ);

			return Topology::ByCoreTopology(pCoreTransformedTopology);
		}*/
	}
}