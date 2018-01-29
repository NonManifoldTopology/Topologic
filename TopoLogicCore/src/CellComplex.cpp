#include <CellComplex.h>

namespace TopoLogicCore
{
	Cell * CellComplex::BoundingCell(CellComplex const * const kpkCellComplex)
	{
		return nullptr;
	}
	void CellComplex::Cells(CellComplex const * const kpkCellComplex, std::list<Cell*>& rCells)
	{
	}
	bool CellComplex::IsClosed(CellComplex const * const kpkCellComplex)
	{
		return false;
	}
	CellComplex * CellComplex::ByCells(const std::list<Cell*>& rkCells)
	{
		return nullptr;
	}
	CellComplex::CellComplex(TopoDS_CompSolid * const kpOcctCompSolid)
		: Topology(3)
		, m_pOcctCompSolid(kpOcctCompSolid)
	{
	}
	CellComplex::~CellComplex()
	{
	}
}
