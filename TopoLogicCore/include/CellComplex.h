#pragma once

#include <Topology.h>

#include <list>

class TopoDS_CompSolid;

namespace TopoLogicCore
{
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class CellComplex : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCellComplex"></param>
		/// <returns></returns>
		static Cell* BoundingCell(CellComplex const * const kpkCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCellComplex"></param>
		/// <param name="rCells"></param>
		static void Cells(CellComplex const * const kpkCellComplex, std::list<Cell*>& rCells);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCellComplex"></param>
		/// <returns></returns>
		static bool IsClosed(CellComplex const * const kpkCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkCells"></param>
		/// <returns></returns>
		static CellComplex* ByCells(const std::list<Cell*>& rkCells);

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctCompSolid"></param>
		CellComplex(TopoDS_CompSolid * const kpOcctCompSolid);
		virtual ~CellComplex();

		/// <summary>
		/// The underlying OCCT cell complex.
		/// </summary>
		TopoDS_CompSolid* m_pOcctCompSolid;
	};
}
