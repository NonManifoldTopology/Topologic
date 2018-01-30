#include <Edge.h>

namespace TopoLogicCore
{
	Vertex * Edge::StartVertex() const
	{
		return nullptr;
	}
	Vertex * Edge::EndVertex() const
	{
		return nullptr;
	}
	void Edge::Wires(std::list<Wire*>& rWires) const
	{
	}
	Edge * Edge::ByCurve(Handle(Geom_Curve) pOcctCurve)
	{
		return nullptr;
	}
	Edge * Edge::ByVertices(const std::list<Vertex*>& rkVertices)
	{
		return nullptr;
	}
	Vertex * Edge::SharedVertex(Edge const * const kpkAnotherEdge) const
	{
		return nullptr;
	}


	void Edge::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{

	}

	Edge::Edge(TopoDS_Edge * const kpOcctEdge)
		: Topology(1)
		, m_pOcctEdge(kpOcctEdge)
	{
	}
	Edge::~Edge()
	{
	}
}
