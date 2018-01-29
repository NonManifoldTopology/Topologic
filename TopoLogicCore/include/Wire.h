#pragma once

#include <Topology.h>

#include <list>

class TopoDS_Wire;

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Face;

	class Wire : Topology
	{
	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <param name="rEdges"></param>
		static void Edges(Wire const * const kpkWire, std::list<Edge*>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <param name="rFaces"></param>
		static void Faces(Wire const * const kpkWire, std::list<Face*>& rFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <returns></returns>
		static bool IsClosed(Wire const * const kpkWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <param name="rVertices"></param>
		static void Vertices(Wire const * const kpkWire, std::list<Vertex*>& rVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static Wire* ByEdges(const std::list<Edge*>& rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <returns></returns>
		static Wire* ByPolyCurve(TopoDS_Wire const * const kpkWire);

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctWire"></param>
		Wire(TopoDS_Wire * const kpOcctWire);
		virtual ~Wire();

		/// <summary>
		/// The underlying OCCT wire.
		/// </summary>
		TopoDS_Wire* m_pOcctWire;
	};
}
