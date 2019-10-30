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

#include "CellComplex.h"

#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <Cell.h>
#include <CellComplex.h>
#include <CellComplexFactory.h>

#include <assert.h>

namespace Topologic
{
	CellComplex^ CellComplex::ByCells(System::Collections::Generic::IEnumerable<Cell^>^ cells)
	{
		std::list<TopologicCore::Cell::Ptr> coreCells;
		for each(Cell^ pCell in cells)
		{
			coreCells.push_back(TopologicCore::Topology::Downcast<TopologicCore::Cell>(pCell->GetCoreTopologicalQuery()));
		}
		try {
			TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::CellComplex::ByCells(coreCells);
			CellComplex^ pCellComplex = gcnew CellComplex(pCoreCellComplex);
			return pCellComplex;
		}
		catch (const std::exception& rkException)
		{
			throw gcnew Exception(gcnew String(rkException.what()));
		}
	}

	CellComplex^ CellComplex::ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces, double tolerance)
	{
		std::list<TopologicCore::Face::Ptr> coreFaces;
		for each(Face^ pFace in faces)
		{
			coreFaces.push_back(TopologicCore::Topology::Downcast<TopologicCore::Face>(pFace->GetCoreTopologicalQuery()));
		}

		try {
			TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::CellComplex::ByFaces(coreFaces, tolerance);
			CellComplex^ pCellComplex = gcnew CellComplex(pCoreCellComplex);
			return pCellComplex;
		}
		catch (const std::exception& rkException)
		{
			throw gcnew Exception(gcnew String(rkException.what()));
		}
	}

	List<Cell^>^ CellComplex::Cells::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Cell::Ptr> coreCells;
		pCoreCellComplex->Cells(coreCells);

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

	List<Face^>^ CellComplex::Faces::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Face::Ptr> coreFaces;
		pCoreCellComplex->Faces(coreFaces);

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

	List<Shell^>^ CellComplex::Shells::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Shell::Ptr> coreShells;
		pCoreCellComplex->Shells(coreShells);

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

	List<Wire^>^ CellComplex::Wires::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Wire::Ptr> coreWires;
		pCoreCellComplex->Wires(coreWires);

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

	List<Edge^>^ CellComplex::Edges::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Edge::Ptr> coreEdges;
		pCoreCellComplex->Edges(coreEdges);

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

	List<Vertex^>^ CellComplex::Vertices::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		pCoreCellComplex->Vertices(coreVertices);

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

	Cell^ CellComplex::ExternalBoundary::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());
		TopologicCore::Cell::Ptr pCoreEnvelope = pCoreCellComplex->ExternalBoundary();
		return gcnew Cell(pCoreEnvelope);
	}

	List<Face^>^ CellComplex::InternalBoundaries::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Face::Ptr> coreInternalFaces;
		pCoreCellComplex->InternalBoundaries(coreInternalFaces);

		List<Face^>^ pInternalFaces = gcnew List<Face^>();
		for (std::list<TopologicCore::Face::Ptr>::const_iterator kInternalFaceIterator = coreInternalFaces.begin();
			kInternalFaceIterator != coreInternalFaces.end();
			kInternalFaceIterator++)
		{
			Face^ pInternalFace = gcnew Face(*kInternalFaceIterator);
			pInternalFaces->Add(pInternalFace);
		}

		return pInternalFaces;
	}

	List<Face^>^ CellComplex::NonManifoldFaces::get()
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = TopologicCore::Topology::Downcast<TopologicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopologicCore::Face::Ptr> coreNonManifoldFaces;
		pCoreCellComplex->NonManifoldFaces(coreNonManifoldFaces);

		List<Face^>^ pNonManifoldFaces = gcnew List<Face^>();
		for (std::list<TopologicCore::Face::Ptr>::const_iterator kNonManifoldFaceIterator = coreNonManifoldFaces.begin();
			kNonManifoldFaceIterator != coreNonManifoldFaces.end();
			kNonManifoldFaceIterator++)
		{
			Face^ pNonManifoldFace = gcnew Face(*kNonManifoldFaceIterator);
			pNonManifoldFaces->Add(pNonManifoldFace);
		}

		return pNonManifoldFaces;
	}

	Object^ CellComplex::BasicGeometry::get()
	{
		List<Cell^>^ pCells = Cells;
		List<Object^>^ pObjects = gcnew List<Object^>();
		for each(Cell^ pCell in pCells)
		{
			pObjects->Add(pCell->BasicGeometry);
		}
		return pObjects;
	}

	int CellComplex::Type()
	{
		return TopologicCore::CellComplex::Type();
	}

	CellComplex::CellComplex()
		: Topologic::Topology()
		, m_pCoreCellComplex(nullptr)
	{

	}

	CellComplex::CellComplex(const TopologicCore::CellComplex::Ptr& kpCoreCellComplex)
		: Topology()
		, m_pCoreCellComplex(kpCoreCellComplex != nullptr ? new TopologicCore::CellComplex::Ptr(kpCoreCellComplex) : throw gcnew Exception("A null cellComplex was created."))
	{

	}

	std::shared_ptr<TopologicCore::TopologicalQuery> CellComplex::GetCoreTopologicalQuery()
	{
		assert(m_pCoreCellComplex != nullptr && "CellComplex::m_pCoreCellComplex is null.");
		if (m_pCoreCellComplex == nullptr)
		{
			throw gcnew Exception("CellComplex::m_pCoreCellComplex is null.");
		}

		return *m_pCoreCellComplex;
	}

	CellComplex::~CellComplex()
	{
		delete m_pCoreCellComplex;
	}
}