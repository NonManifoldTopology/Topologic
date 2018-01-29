#pragma once

#include <Topology.h>

#include <list>

#include <Standard_Handle.hxx>

class Geom_Surface;
class TopoDS_Face;

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
		/// <param name="kpkFace"></param>
		/// <param name="rFaces"></param>
		static void AdjacentFaces(Face const * const kpkFace, std::list<Face*>& rFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <param name="rFaces"></param>
		static void Apertures(Face const * const kpkFace, std::list<Face*>& rFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <param name="rCells"></param>
		static void Cells(Face const * const kpkFace, std::list<Cell*>& rCells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <param name="rEdges"></param>
		static void Edges(Face const * const kpkFace, std::list<Edge*>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <returns></returns>
		static bool IsApplied(Face const * const kpkFace);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <param name="rShells"></param>
		static void Shells(Face const * const kpkFace, std::list<Shell*>& rShells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <param name="rVertices"></param>
		static void Vertices(Face const * const kpkFace, std::list<Vertex*>& rVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace"></param>
		/// <param name="rWires"></param>
		static void Wires(Face const * const kpkFace, std::list<Wire*>& rWires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkApertures"></param>
		/// <returns></returns>
		static bool ApplyApertures(const std::list<Face*>& rkApertures);

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
		/// <param name="kpkFace1"></param>
		/// <param name="kpkFace2"></param>
		/// <param name="rEdges"></param>
		static void SharedEdges(Face const * const kpkFace1, Face const * const kpkFace2, std::list<Edge*>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkFace1"></param>
		/// <param name="kpkFace2"></param>
		/// <param name="rVertices"></param>
		static void SharedVertices(Face const * const kpkFace1, Face const * const kpkFace2, std::list<Vertex*>& rVertices);

	protected:
		Face(TopoDS_Face * const kpOcctFace);
		virtual ~Face();

		/// <summary>
		/// The underlying OCCT face.
		/// </summary>
		TopoDS_Face* m_pOcctFace;
	};
}
