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
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "Cluster.h"

#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <Cell.h>
#include <CellComplex.h>

#include <assert.h>

namespace Topologic
{
	Cluster^ Cluster::ByTopologies(System::Collections::Generic::IEnumerable<Topology^>^ topologies)
	{
		std::list<std::shared_ptr<TopologicCore::Topology>> coreTopologies;
		for each(Topology^ topology in topologies)
		{
			std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			if(pCoreTopology != nullptr)
			{
				coreTopologies.push_back(pCoreTopology);
			}
		}
		
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Cluster::ByTopologies(coreTopologies);
		return gcnew Cluster(pCoreCluster);
	}

	Cluster^ Cluster::AddTopology(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		if (!pCoreCluster->AddTopology(pCoreTopology.get()))
		{
			throw gcnew Exception("Cluster::Add(): fails to add topology");
		}
		return this;
	}

	Cluster^ Cluster::RemoveTopology(Topology^ topology)
	{
		std::shared_ptr<TopologicCore::Topology> pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());
		if (!pCoreCluster->RemoveTopology(pCoreTopology.get()))
		{
			throw gcnew Exception("Cluster::Remove(): fails to remove topology");
		}
		return this;
	}

	Object^ Cluster::BasicGeometry::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopologicCore::Topology>> coreSubTopologies;
		pCoreCluster->SubTopologies(coreSubTopologies);

		List<Object^>^ pTopologies = gcnew List<Object^>();

		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kCoreIterator = coreSubTopologies.begin();
			kCoreIterator != coreSubTopologies.end();
			kCoreIterator++)
		{
			pTopologies->Add(Topology::ByCoreTopology(*kCoreIterator)->BasicGeometry);
		}

		return pTopologies;
	}

	int Cluster::Type()
	{
		return TopologicCore::Cluster::Type();
	}

	Cluster::Cluster(const TopologicCore::Cluster::Ptr& kpCoreCluster)
		: Topology()
		, m_pCoreCluster(kpCoreCluster != nullptr ? new TopologicCore::Cluster::Ptr(kpCoreCluster) : throw gcnew Exception("A null cluster was created."))
	{

	}

	Cluster::Cluster()
		: Topology()
		, m_pCoreCluster(nullptr)
	{
		
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Cluster::GetCoreTopologicalQuery()
	{
		assert(m_pCoreCluster != nullptr && "Cluster::m_pCoreCluster is null.");
		if (m_pCoreCluster == nullptr)
		{
			throw gcnew Exception("Cluster::m_pCoreCluster is null.");
		}

		return *m_pCoreCluster;
	}

	Cluster::~Cluster()
	{
		delete m_pCoreCluster;
	}


	List<Shell^>^ Cluster::Shells::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Shell::Ptr> coreShells;
		pCoreCluster->Shells(coreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		for (std::list<TopologicCore::Shell::Ptr>::const_iterator kShellIterator = coreShells.begin();
			kShellIterator != coreShells.end();
			kShellIterator++)
		{
			Shell^ pShell = gcnew Shell(*kShellIterator);
			pShells->Add(pShell);
		}

		return pShells;
	}

	List<Face^>^ Cluster::Faces::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Face::Ptr> coreFaces;
		pCoreCluster->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopologicCore::Face::Ptr>::const_iterator kFaceIterator = coreFaces.begin();
			kFaceIterator != coreFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	List<Wire^>^ Cluster::Wires::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Wire::Ptr> coreWires;
		pCoreCluster->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopologicCore::Wire::Ptr>::const_iterator kWireIterator = coreWires.begin();
			kWireIterator != coreWires.end();
			kWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kWireIterator);
			pWires->Add(pWire);
		}

		return pWires;
	}

	List<Edge^>^ Cluster::Edges::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Edge::Ptr> coreEdges;
		pCoreCluster->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopologicCore::Edge::Ptr>::const_iterator kEdgeIterator = coreEdges.begin();
			kEdgeIterator != coreEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Vertex^>^ Cluster::Vertices::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		pCoreCluster->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopologicCore::Vertex::Ptr>::const_iterator kVertexIterator = coreVertices.begin();
			kVertexIterator != coreVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	List<Cell^>^ Cluster::Cells::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Cell::Ptr> coreCells;
		pCoreCluster->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopologicCore::Cell::Ptr>::const_iterator kCellIterator = coreCells.begin();
			kCellIterator != coreCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<CellComplex^>^ Cluster::CellComplexes::get()
	{
		TopologicCore::Cluster::Ptr pCoreCluster = TopologicCore::Topology::Downcast<TopologicCore::Cluster>(GetCoreTopologicalQuery());

		std::list<TopologicCore::CellComplex::Ptr> coreCellComplexes;
		pCoreCluster->CellComplexes(coreCellComplexes);

		List<CellComplex^>^ pCellComplexes = gcnew List<CellComplex^>();
		for (std::list<TopologicCore::CellComplex::Ptr>::const_iterator kCellComplexIterator = coreCellComplexes.begin();
			kCellComplexIterator != coreCellComplexes.end();
			kCellComplexIterator++)
		{
			CellComplex^ pCellComplex = gcnew CellComplex(*kCellComplexIterator);
			pCellComplexes->Add(pCellComplex);
		}

		return pCellComplexes;
	}

}