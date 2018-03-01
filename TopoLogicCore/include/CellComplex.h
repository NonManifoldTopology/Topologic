#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_CompSolid.hxx>

namespace TopoLogicCore
{
	class Face;
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class CellComplex : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctCompSolid"></param>
		CellComplex(const TopoDS_CompSolid& rkOcctCompSolid);

		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Cells(std::list<Cell*>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Faces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API  bool IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkCells"></param>
		/// <returns></returns>
		static TOPOLOGIC_API CellComplex* ByCells(const std::list<Cell*>& rkCells);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Cell* Envelope() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkInternalFaces"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InternalFaces(std::list<Face*>& rInternalFaces) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CELLCOMPLEX; }

	protected:
		/// <summary>
		/// The underlying OCCT cell complex.
		/// </summary>
		TopoDS_CompSolid* m_pOcctCompSolid;
	};
}
