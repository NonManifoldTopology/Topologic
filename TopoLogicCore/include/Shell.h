#pragma once

#include <Topology.h>

#include <list>

class TopoDS_Shell;

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Face;
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class Shell : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShell"></param>
		/// <returns></returns>
		static Cell* Cell(Shell const * const kpkShell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShell"></param>
		/// <param name="rEdges"></param>
		static void Edges(Shell const * const kpkShell, std::list<Edge*>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShell"></param>
		/// <param name="rFaces"></param>
		static void Faces(Shell const * const kpkShell, std::list<Face*>& rFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShell"></param>
		/// <returns></returns>
		static bool IsClosed(Shell const * const kpkShell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkShell"></param>
		/// <param name="rVertices"></param>
		static void Vertices(Shell const * const kpkShell, std::list<Vertex*>& rVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static Shell* ByFaces(const std::list<Face*>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkOcctShell"></param>
		/// <returns></returns>
		static Shell* ByPolySurface(TopoDS_Shell const * const kpkOcctShell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static Shell* ByVerticesFaceIndices(const std::list<Vertex*>& rkVertices, const std::list<int>& rkFaceIndices);

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctShell"></param>
		Shell(TopoDS_Shell * const kpOcctShell);
		virtual ~Shell();

		/// <summary>
		/// The underlying OCCT shell.
		/// </summary>
		TopoDS_Shell* m_pOcctShell;
	};
}
