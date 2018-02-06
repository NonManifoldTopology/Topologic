#include <CellComplex.h>
#include <Face.h>
#include <Cell.h>

#include <assert.h>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ CellComplex::ByCells(List<Cell^>^ cells)
	{
		std::list<TopoLogicCore::Cell*> coreCells;
		for each(Cell^ pCell in cells)
		{
			coreCells.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Cell>(pCell->GetCoreTopology()));
		}
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::CellComplex::ByCells(coreCells);
		CellComplex^ pCellComplex = gcnew CellComplex(pCoreCellComplex);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic CellComplex", pCellComplex);
		pDictionary->Add("Solids", pCellComplex->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ CellComplex::Cells(CellComplex^ cellComplex)
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(cellComplex->GetCoreTopology());

		std::list<TopoLogicCore::Cell*> coreCells;
		pCoreCellComplex->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCellIterator = coreCells.begin();
			kCellIterator != coreCells.end();
			kCellIterator++)
		{
			Cell^ pFace = gcnew Cell(*kCellIterator);
			pCells->Add(pFace);
			pDynamoEntities->Add(pFace->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pCells);
		pDictionary->Add("Surfaces", pDynamoEntities);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ CellComplex::Faces(CellComplex^ cellComplex)
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(cellComplex->GetCoreTopology());

		std::list<TopoLogicCore::Face*> coreFaces;
		pCoreCellComplex->Faces(coreFaces);

		List<Face^>^ pFaces = gcnew List<Face^>();
		List<System::Object^>^ pDynamoEntities = gcnew List<System::Object^>();
		for (std::list<TopoLogicCore::Face*>::const_iterator kFaceIterator = coreFaces.begin();
			kFaceIterator != coreFaces.end();
			kFaceIterator++)
		{
			Face^ pFace = gcnew Face(*kFaceIterator);
			pFaces->Add(pFace);
			pDynamoEntities->Add(pFace->Geometry);
		}

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Faces", pFaces);
		pDictionary->Add("Surfaces", pDynamoEntities);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ CellComplex::BoundingCell(CellComplex^ cellComplex)
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(cellComplex->GetCoreTopology());
		TopoLogicCore::Cell* pCoreBoundingCell = pCoreCellComplex->BoundingCell();
		Cell^ pBoundingCell = gcnew Cell(pCoreBoundingCell);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Cell", pBoundingCell);
		pDictionary->Add("Solid", pBoundingCell->Geometry);
		return pDictionary;
	}

	bool CellComplex::IsClosed::get()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopology());
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

	TopoLogicCore::Topology* CellComplex::GetCoreTopology()
	{
		assert(m_pCoreCellComplex != nullptr && "CellComplex::m_pCoreCellComplex is null.");
		if (m_pCoreCellComplex == nullptr)
		{
			throw gcnew Exception("CellComplex::m_pCoreCellComplex is null.");
		}

		return m_pCoreCellComplex;
	}

	List<Cell^>^ CellComplex::Cells()
	{
		TopoLogicCore::CellComplex* pCoreCellComplex = TopoLogicCore::Topology::Downcast<TopoLogicCore::CellComplex>(GetCoreTopology());
		std::list<TopoLogicCore::Cell*> coreCells;
		pCoreCellComplex->Cells(coreCells);

		List<Cell^>^ pCells = gcnew List<Cell^>();
		for (std::list<TopoLogicCore::Cell*>::const_iterator kCoreCellIterator = coreCells.begin();
			kCoreCellIterator != coreCells.end();
			kCoreCellIterator++)
		{
			Cell^ pCell = gcnew Cell(*kCoreCellIterator);
			pCells->Add(pCell);
		}

		return pCells;
	}

	CellComplex::~CellComplex()
	{
		delete m_pCoreCellComplex;
	}
}