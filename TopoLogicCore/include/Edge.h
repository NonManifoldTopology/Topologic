#pragma once

#include "Topology.h"

#include <list>

#include <Standard_Handle.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS_Edge.hxx>

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
		/// <param name="kpOcctEdge"></param>
		Edge(TopoDS_Edge * const kpOcctEdge);

		virtual ~Edge();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Vertex* StartVertex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Vertex* EndVertex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		void Wires(std::list<Wire*>& rWires) const;

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
		/// <param name="kpkAnotherEdge"></param>
		/// <returns></returns>
		Vertex* SharedVertex(Edge const * const kpkAnotherEdge) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctEdge; }

	protected:

		/// <summary>
		/// The underlying OCCT edge.
		/// </summary>
		TopoDS_Edge* m_pOcctEdge;
	};
}
