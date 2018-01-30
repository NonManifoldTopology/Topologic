#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_Wire.hxx>

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
		/// <param name="kpOcctWire"></param>
		Wire(TopoDS_Wire * const kpOcctWire);

		virtual ~Wire();

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
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static Wire* ByEdges(const std::list<Edge*>& rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <returns></returns>
		static Wire* ByPolyCurve(TopoDS_Wire const * const kpkWire);

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctWire; }

	protected:
		/// <summary>
		/// The underlying OCCT wire.
		/// </summary>
		TopoDS_Wire* m_pOcctWire;
	};
}
