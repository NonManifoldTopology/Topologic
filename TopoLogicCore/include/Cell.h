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
		/// <param name="rcells"></param>
		TOPOLOGIC_API void AdjacentCells(std::list<Cell*>& rcells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API CellComplex* CellComplex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		TOPOLOGIC_API void Shells(std::list<Shell*>& rShells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void Faces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void Vertices(std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<Wire*>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		TOPOLOGIC_API double Volume() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Cell* ByFaces(const std::list<Face*>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShells"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Cell* ByShell(Shell const * const kpkShells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCentroid"></param>
		/// <param name="kXDimension"></param>
		/// <param name="kYDimension"></param>
		/// <param name="kZDimension"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Cell* ByCuboid(Handle(Geom_CartesianPoint) pOcctCentroid, const double kXDimension, const double kYDimension, const double kZDimension);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Cell* ByVerticesFaceIndices(const std::vector<Vertex*>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherCell"></param>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void SharedEdges(Cell const * const kpkAnotherCell, std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherCell"></param>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void SharedFaces(Cell const * const kpkAnotherCell, std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherCell"></param>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void SharedVertices(Cell const * const kpkAnotherCell, std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CELL; }

		void InnerShells(std::list<Shell*>& rShells) const;

	protected:
		/// <summary>
		/// The underlying OCCT cell.
		/// </summary>
		TopoDS_Solid* m_pOcctSolid;
	};
}
