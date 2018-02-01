#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_Shell.hxx>

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Face;
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class Shell : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctShell"></param>
		Shell(TopoDS_Shell * const kpOcctShell);

		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Cell* Cell() const;

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
		/// <returns></returns>
		bool IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		void Vertices(std::list<Vertex*>& rVertices) const;

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

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctShell; }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

	protected:
		/// <summary>
		/// The underlying OCCT shell.
		/// </summary>
		TopoDS_Shell* m_pOcctShell;
	};
}
