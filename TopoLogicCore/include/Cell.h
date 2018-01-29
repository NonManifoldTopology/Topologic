#pragma once

#include <Topology.h>

#include <list>

class TopoDS_Solid;

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;
	class CellComplex;

	class Cell : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell"></param>
		/// <param name="rcells"></param>
		static void AdjacentCells(Cell const * const kpkCell, std::list<Cell*>& rcells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell"></param>
		/// <returns></returns>
		static CellComplex* CellComplex(Cell const * const kpkCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell"></param>
		/// <param name="rEdges"></param>
		static void Edges(Cell const * const kpkCell, std::list<Edge*>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell"></param>
		/// <param name="rFaces"></param>
		static void Faces(Cell const * const kpkCell, std::list<Face*>& rFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell"></param>
		/// <param name="rVertices"></param>
		static void Vertices(Cell const * const kpkCell, std::list<Vertex*>& rVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell"></param>
		/// <param name="rWires"></param>
		static void Wires(Cell const * const kpkCell, std::list<Wire*>& rWires);

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
		/// <param name="kpkCell1"></param>
		/// <param name="kpkCell12"></param>
		/// <param name="rEdges"></param>
		static void SharedEdges(Cell const * const kpkCell1, Cell const * const kpkCell12, std::list<Edge*>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell1"></param>
		/// <param name="kpkCell12"></param>
		/// <param name="rFaces"></param>
		static void SharedFaces(Cell const * const kpkCell1, Cell const * const kpkCell12, std::list<Face*>& rFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkCell1"></param>
		/// <param name="kpkCell12"></param>
		/// <param name="rVertices"></param>
		static void SharedVertices(Cell const * const kpkCell1, Cell const * const kpkCell2, std::list<Vertex*>& rVertices);

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctCell"></param>
		Cell(TopoDS_Solid * const kpOcctSolid);
		virtual ~Cell();

		/// <summary>
		/// The underlying OCCT cell.
		/// </summary>
		TopoDS_Solid* m_pOcctSolid;
	};
}
