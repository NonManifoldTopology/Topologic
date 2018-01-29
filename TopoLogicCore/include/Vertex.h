#pragma once

#include <Topology.h>

#include <Standard_Handle.hxx>
#include <Geom_Geometry.hxx>

class TopoDS_Vertex;
class Geom_Point;
class gp_Pnt;

namespace TopoLogicCore
{
	class Edge;

	/// <summary>
	/// 
	/// </summary>
	class Vertex : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkPoint"></param>
		/// <returns></returns>
		static Vertex* ByPoint(Handle(Geom_Point) pOcctPoint);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		void Edges(std::list<Edge*>& rEdges);


		/// <summary>
		/// 
		/// </summary>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

	protected:
		/// <summary>
		/// Creates a vertex by an OCCT vertex.
		/// </summary>
		/// <param name="kpOcctVertex">An OCCT vertex</param>
		Vertex(TopoDS_Vertex * const kpOcctVertex);

		virtual ~Vertex();

		/// <summary>
		/// The underlying OCCT vertex.
		/// </summary>
		TopoDS_Vertex* m_pOcctVertex;
	};
}