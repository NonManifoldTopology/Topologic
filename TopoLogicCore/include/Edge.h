#pragma once

#include <Topology.h>

#include <list>

#include <Standard_Handle.hxx>
#include <Geom_Curve.hxx>

class TopoDS_Edge;

namespace TopoLogicCore
{
	class Vertex;
	class Wire;

	class Edge : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		static Vertex* StartVertex(Edge const * const kpkEdge);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		static Vertex* EndVertex(Edge const * const kpkEdge);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		static void Wires(Edge const * const kpkEdge, std::list<Wire*>& rWires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCurve"></param>
		/// <returns></returns>
		static Edge* ByCurve(Handle(Geom_Curve) pOcctCurve);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <returns></returns>
		static Edge* ByVertices(const std::list<Vertex*>& rkVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkEdge1"></param>
		/// <param name="kpkEdge2"></param>
		/// <returns></returns>
		static Vertex* SharedVertex(Edge const * const kpkEdge1, Edge const * const kpkEdge2);

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctEdge"></param>
		Edge(TopoDS_Edge * const kpOcctEdge);
		virtual ~Edge();

		/// <summary>
		/// The underlying OCCT edge.
		/// </summary>
		TopoDS_Edge* m_pOcctEdge;
	};
}
