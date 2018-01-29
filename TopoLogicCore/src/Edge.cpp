#include <Edge.h>

namespace TopoLogicCore
{
	Vertex * Edge::StartVertex(Edge const * const kpkEdge)
	{
		return nullptr;
	}
	Vertex * Edge::EndVertex(Edge const * const kpkEdge)
	{
		return nullptr;
	}
	void Edge::Wires(Edge const * const kpkEdge, std::list<Wire*>& rWires)
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
	Vertex * Edge::SharedVertex(Edge const * const kpkEdge1, Edge const * const kpkEdge2)
	{
		return nullptr;
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
