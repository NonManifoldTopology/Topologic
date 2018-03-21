#include "CellComplex.h"

#include <Vertex.h>
#include <Edge.h>
#include <Wire.h>
#include <Face.h>
#include <Shell.h>
#include <Cell.h>
#include <CellComplex.h>

#include <assert.h>

namespace TopoLogic
{
	CellComplex^ CellComplex::ByCells(List<Cell^>^ cells)
	{
		std::list<TopoLogicCore::Cell*> coreCells;
		for each(Cell^ pCell in cells)
		{
			coreCells.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(pCell->GetCoreTopologicalQuery()));
		}
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::CellComplex::ByCells(coreCells);
		CellComplex^ pCellComplex = gcnew CellComplex(pCoreCellComplex);
		return pCellComplex;
	}

	List<Cell^>^ CellComplex::Cells()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Cell*> coreCells;
		pCoreCellComplex->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = coreCells.begin();
			kCellIterator != coreCells.end();
			kCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}

	List<Face^>^ CellComplex::Faces()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreCellComplex->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kFaceIterator = coreFaces.begin();
			kFaceIterator != coreFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pFaces->Add(pFace);
		}

		return pFaces;
	}

	List<Shell^>^ CellComplex::Shells()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Shell*> coreShells;
		pCoreCellComplex->Shells(coreShells);

		List<Shell^>^ pShells = gcnew List<Shell^>();
		for (std::list<TopoLogicCore::Shell*>::const_iterator kShellIterator = coreShells.begin();
			kShellIterator != coreShells.end();
			kShellIterator++)
		{
			Shell^ pShell = gcnew Shell(*kShellIterator);
			pShells->Add(pShell);
		}

		return pShells;
	}

	List<Wire^>^ CellComplex::Wires()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Wire*> coreWires;
		pCoreCellComplex->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for (std::list<TopoLogicCore::Wire*>::const_iterator kWireIterator = coreWires.begin();
			kWireIterator != coreWires.end();
			kWireIterator++)
		{
			Wire^ pWire = gcnew Wire(*kWireIterator);
			pWires->Add(pWire);
		}

		return pWires;
	}

	List<Edge^>^ CellComplex::Edges()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Edge*> coreEdges;
		pCoreCellComplex->Edges(coreEdges);

		List<Edge^>^ pEdges = gcnew List<Edge^>();
		for (std::list<TopoLogicCore::Edge*>::const_iterator kEdgeIterator = coreEdges.begin();
			kEdgeIterator != coreEdges.end();
			kEdgeIterator++)
		{
			Edge^ pEdge = gcnew Edge(*kEdgeIterator);
			pEdges->Add(pEdge);
		}

		return pEdges;
	}

	List<Vertex^>^ CellComplex::Vertices()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Vertex*> coreVertices;
		pCoreCellComplex->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopoLogicCore::Vertex*>::const_iterator kVertexIterator = coreVertices.begin();
			kVertexIterator != coreVertices.end();
			kVertexIterator++)
		{
			Vertex^ pVertex = gcnew Vertex(*kVertexIterator);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	Cell^ CellComplex::OuterBoundary()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());
		TopoLogicCore::Cell* pCoreEnvelope = pCoreCellComplex->OuterBoundary();
		return gcnew Cell(pCoreEnvelope);
	}

	List<Face^>^ CellComplex::InnerBoundaries()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Face*> coreInternalFaces;
		pCoreCellComplex->InnerBoundaries(coreInternalFaces);

		List<Face^>^ pInternalFaces = gcnew List<Face^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kInternalFaceIterator = coreInternalFaces.begin();
			kInternalFaceIterator != coreInternalFaces.end();
			kInternalFaceIterator++)
		{
			Face^ pInternalFace = gcnew Face(*kInternalFaceIterator);
			pInternalFaces->Add(pInternalFace);
		}

		return pInternalFaces;
	}

	Object^ CellComplex::Geometry::get()
	{
		List<Cell^>^ pCells = Cells();
		List<Object^>^ pObjects = gcnew List<Object^>();
		for each(Cell^ pCell in pCells)
		{
			pObjects->Add(pCell->Geometry);
		}
		return pObjects;
	}

	CellComplex::CellComplex(TopoLogicCore::CellComplex* const kpCoreCellComplex)
		: Topology()
		, m_pCoreCellComplex(kpCoreCellComplex)
	{

	}

	TopoLogicCore::TopologicalQuery* CellComplex::GetCoreTopologicalQuery()
	{
		assert(m_pCoreCellComplex != nullptr && "CellComplex::m_pCoreCellComplex is null.");
		if (m_pCoreCellComplex == nullptr)
		{
			throw gcnew Exception("CellComplex::m_pCoreCellComplex is null.");
		}

		return m_pCoreCellComplex;
	}

	CellComplex::~CellComplex()
	{
		delete m_pCoreCellComplex;
	}
}