#pragma once

#include "Topology.h"

#include <list>

#include <Standard_Handle.hxx>
#include <TopoDS_Face.hxx>

class Geom_Surface;

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Shell;
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class Face : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctFace"></param>
		Face(TopoDS_Face * const kpOcctFace);

		virtual ~Face();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		void AdjacentFaces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		void Apertures(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		void Cells(std::list<Cell*>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		void Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool IsApplied() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		void Shells(std::list<Shell*>& rShells) const;

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
		/// <param name="rkApertures"></param>
		/// <returns></returns>
		bool ApplyApertures(const std::list<Face*>& rkApertures) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <returns></returns>
		static Face* ByClosedWire(Wire const * const kpkWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static Face* ByEdges(const std::list<Edge*>* rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctSurface"></param>
		/// <returns></returns>
		static Face* BySurface(Handle(Geom_Surface) pOcctSurface);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherFace"></param>
		/// <param name="rEdges"></param>
		void SharedEdges(Face const * const kpkAnotherFace, std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherFace"></param>
		/// <param name="rVertices"></param>
		void SharedVertices(Face const * const kpkAnotherFace, std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctFace; }

	protected:
		/// <summary>
		/// The underlying OCCT face.
		/// </summary>
		TopoDS_Face* m_pOcctFace;
	};
}
