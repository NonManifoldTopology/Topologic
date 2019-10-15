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

#include <Utilities/WireUtility.h>
#include <Utilities/EdgeUtility.h>

#include <Edge.h>
#include <Vertex.h>

namespace TopologicUtilities
{
	void WireUtility::AdjacentShells(
		const TopologicCore::Wire::Ptr & kpWire,
		const TopologicCore::Topology::Ptr & kpParentTopology,
		std::list<TopologicCore::Shell::Ptr>& rCoreAdjacentShells)
	{
		std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
		kpWire->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Shell::Type(), coreAdjacentTopologies);
		for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
		{
			rCoreAdjacentShells.push_back(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Shell>(kpAdjacentTopology)
			);
		}
	}

	void TopologicUtilities::WireUtility::AdjacentCells(const TopologicCore::Wire::Ptr & kpWire, const TopologicCore::Topology::Ptr & kpParentTopology, std::list<TopologicCore::Cell::Ptr>& rCoreAdjacentCells)
	{
		std::list<TopologicCore::Topology::Ptr> coreAdjacentTopologies;
		kpWire->UpwardNavigation(kpParentTopology->GetOcctShape(), TopologicCore::Cell::Type(), coreAdjacentTopologies);
		for (const TopologicCore::Topology::Ptr& kpAdjacentTopology : coreAdjacentTopologies)
		{
			rCoreAdjacentCells.push_back(
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Cell>(kpAdjacentTopology)
			);
		}
	}



	TopologicCore::Wire::Ptr TopologicUtilities::WireUtility::RemoveCollinearEdges(const TopologicCore::Wire::Ptr & kpWire, const double kTolerance)
	{
		if (kpWire->NumberOfBranches() > 0)
		{
			throw std::exception("This method currently only supportes straight, manifold wires with no branches.");
		}

		TopologicCore::Wire::Ptr pCopyWire = std::dynamic_pointer_cast<TopologicCore::Wire>(kpWire->DeepCopy());

		std::list<TopologicCore::Edge::Ptr> edges;
		pCopyWire->Edges(edges);

		if (edges.empty())
		{
			return nullptr;
		}

		std::list<TopologicCore::Vertex::Ptr> vertices;

		if (!kpWire->IsClosed())
		{
			vertices.push_back((*edges.begin())->StartVertex());
		}

		std::list<TopologicCore::Edge::Ptr>::iterator secondLastEdge = edges.end();
		secondLastEdge--;

		for (std::list<TopologicCore::Edge::Ptr>::iterator currentEdgeIterator = edges.begin();
			currentEdgeIterator != secondLastEdge;
			currentEdgeIterator++)
		{
			std::list<TopologicCore::Edge::Ptr>::iterator nextEdgeIterator = currentEdgeIterator;
			nextEdgeIterator++;

			TopologicCore::Edge::Ptr currentEdge = *currentEdgeIterator;
			TopologicCore::Edge::Ptr nextEdge = *nextEdgeIterator;

			double angle = EdgeUtility::AngleBetween(currentEdge, nextEdge);
			if (std::abs(angle) > kTolerance)
			{
				vertices.push_back(currentEdge->EndVertex());
			}
		}

		if (kpWire->IsClosed())
		{
			TopologicCore::Edge::Ptr currentEdge = *secondLastEdge;
			TopologicCore::Edge::Ptr nextEdge = *edges.begin();

			double angle = EdgeUtility::AngleBetween(currentEdge, nextEdge);
			if (std::abs(angle) > kTolerance)
			{
				vertices.push_back(nextEdge->StartVertex());
			}
		}
		else
		{
			vertices.push_back((*secondLastEdge)->EndVertex());
		}

		std::list<TopologicCore::Vertex::Ptr>::iterator secondLastVertex = vertices.end();
		secondLastVertex--;

		std::list<TopologicCore::Edge::Ptr> newEdges;
		for (std::list<TopologicCore::Vertex::Ptr>::iterator currentVertexIterator = vertices.begin();
			currentVertexIterator != secondLastVertex;
			currentVertexIterator++)
		{
			std::list<TopologicCore::Vertex::Ptr>::iterator nextVertexIterator = currentVertexIterator;
			nextVertexIterator++;

			TopologicCore::Vertex::Ptr currentVertex = *currentVertexIterator;
			TopologicCore::Vertex::Ptr nextVertex = *nextVertexIterator;

			TopologicCore::Edge::Ptr newEdge = TopologicCore::Edge::ByStartVertexEndVertex(currentVertex, nextVertex);
			newEdges.push_back(newEdge);
		}

		if (kpWire->IsClosed())
		{
			TopologicCore::Vertex::Ptr currentVertex = *secondLastVertex;
			TopologicCore::Vertex::Ptr nextVertex = *vertices.begin();

			TopologicCore::Edge::Ptr newEdge = TopologicCore::Edge::ByStartVertexEndVertex(currentVertex, nextVertex);
			newEdges.push_back(newEdge);
		}

		return TopologicCore::Wire::ByEdges(newEdges);
	}
}