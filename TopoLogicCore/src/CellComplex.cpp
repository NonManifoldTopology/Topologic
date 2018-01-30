#include <CellComplex.h>

namespace TopoLogicCore
{
	Cell * CellComplex::BoundingCell() const
	{
		return nullptr;
	}
	void CellComplex::Cells(std::list<Cell*>& rCells) const
	{
	}
	bool CellComplex::IsClosed() const
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
