#include "CellComplex.h"
#include <Face.h>
#include <Cell.h>

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

	Cell^ CellComplex::Envelope()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());
		TopoLogicCore::Cell* pCoreEnvelope = pCoreCellComplex->Envelope();
		return gcnew Cell(pCoreEnvelope);
	}

	List<Face^>^ CellComplex::InternalFaces()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());

		std::list<TopoLogicCore::Face*> coreInternalFaces;
		pCoreCellComplex->InternalFaces(coreInternalFaces);

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

	bool CellComplex::IsClosed::get()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopologicalQuery());
		return pCoreCellComplex->IsClosed();
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