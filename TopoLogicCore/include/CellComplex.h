#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_CompSolid.hxx>

namespace TopologicCore
{
	class Cell;
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;

	/// <summary>
	/// 
	/// </summary>
	class CellComplex : public Topology
	{
	public:
		typedef std::shared_ptr<CellComplex> Ptr;

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
		TOPOLOGIC_API void Cells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Faces(std::list<std::shared_ptr<Face>>& rFaces) const;
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		TOPOLOGIC_API void Shells(std::list<std::shared_ptr<Shell>>& rShells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<std::shared_ptr<Wire>>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkCells"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<CellComplex> ByCells(const std::list<std::shared_ptr<Cell>>& rkCells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<CellComplex> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Cell> OuterBoundary() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkInternalFaces"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InnerBoundaries(std::list<std::shared_ptr<Face>>& rInternalFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rNonManifoldFaces"></param>
		/// <returns></returns>
		TOPOLOGIC_API void NonManifoldFaces(std::list<std::shared_ptr<Face>>& rNonManifoldFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shape& GetOcctShape();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Shape& GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_CompSolid& GetOcctCompSolid();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_CompSolid& GetOcctCompSolid() const;

		/// <summary>
		/// 
		/// </summary>
		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CELLCOMPLEX; }

		virtual std::string GetGUID() const {
			return std::string("4ec9904b-dc01-42df-9647-2e58c2e08e78");
		}

	protected:
		/// <summary>
		/// The underlying OCCT cell complex.
		/// </summary>
		TopoDS_CompSolid m_pOcctCompSolid;
	};
}
