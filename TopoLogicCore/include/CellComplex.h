#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_CompSolid.hxx>

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
		/// <param name="kpOcctCompSolid"></param>
		CellComplex(TopoDS_CompSolid * const kpOcctCompSolid);

		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Cell* BoundingCell() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		void Cells(std::list<Cell*>& rCells) const;
		
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkCells"></param>
		/// <returns></returns>
		static CellComplex* ByCells(const std::list<Cell*>& rkCells);

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctCompSolid; }


	protected:
		/// <summary>
		/// The underlying OCCT cell complex.
		/// </summary>
		TopoDS_CompSolid* m_pOcctCompSolid;
	};
}
