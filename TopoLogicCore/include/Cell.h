#pragma once

#include "Topology.h"

#include <list>
#include <vector>

#include <Geom_CartesianPoint.hxx>
#include <TopoDS_Solid.hxx>

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;
	class CellComplex;

	class Cell : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctSolid"></param>
		Cell(const TopoDS_Solid& rkOcctSolid);
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void AdjacentCells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCellComplexes"></param>
		TOPOLOGIC_API void CellComplexes(std::list<std::shared_ptr<CellComplex>>& rCellComplexes) const;

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
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void Faces(std::list<std::shared_ptr<Face>>& rFaces) const;

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
		TOPOLOGIC_API double Volume() const;

		/// <summary>
		/// 
		/// </summary>
		TOPOLOGIC_API std::shared_ptr<Vertex> CenterOfMass() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cell> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkSHell"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cell> ByShell(const std::shared_ptr<Shell>& rkShell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCentroid"></param>
		/// <param name="kXDimension"></param>
		/// <param name="kYDimension"></param>
		/// <param name="kZDimension"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cell> ByCuboid(Handle(Geom_CartesianPoint) pOcctCentroid, const double kXDimension, const double kYDimension, const double kZDimension);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cell> ByVerticesFaceIndices(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherCell"></param>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void SharedEdges(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherCell"></param>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void SharedFaces(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherCell"></param>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void SharedVertices(const std::shared_ptr<Cell>& kpkAnotherCell, std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Shell> OuterBoundary() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpVertex"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool DoesContain(const std::shared_ptr<Vertex>& kpVertex) const;


		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InnerBoundaries(std::list<std::shared_ptr<Shell>>& rShells) const;

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
		virtual TopoDS_Solid& GetOcctSolid();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Solid& GetOcctSolid() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CELL; }

	protected:
		/// <summary>
		/// The underlying OCCT cell.
		/// </summary>
		TopoDS_Solid m_occtSolid;
	};
}
