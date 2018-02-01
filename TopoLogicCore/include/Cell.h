#pragma once

#include "Topology.h"

#include <list>

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
		/// <param name="kpOcctCell"></param>
		Cell(TopoDS_Solid * const kpOcctSolid);
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rcells"></param>
		void AdjacentCells(std::list<Cell*>& rcells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		CellComplex* CellComplex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		void Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		void Faces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		void Vertices(std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		void Wires(std::list<Wire*>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOcctCell"></param>
		/// <returns></returns>
		static Cell* BySolid(TopoDS_Solid const * const kpkOcctCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static Cell* ByFace(const std::list<Face*>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShells"></param>
		/// <returns></returns>
		static Cell* ByShell(Shell const * const kpkShells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static Cell* ByVerticesFaceIndices(const std::list<Vertex*>& rkVertices, const std::list<int>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherCell"></param>
		/// <param name="rEdges"></param>
		void SharedEdges(Cell const * const kpkAnotherCell, std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherCell"></param>
		/// <param name="rFaces"></param>
		void SharedFaces(Cell const * const kpkAnotherCell, std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherCell"></param>
		/// <param name="rVertices"></param>
		void SharedVertices(Cell const * const kpkAnotherCell, std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctSolid; }
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

	protected:
		/// <summary>
		/// The underlying OCCT cell.
		/// </summary>
		TopoDS_Solid* m_pOcctSolid;
	};
}
