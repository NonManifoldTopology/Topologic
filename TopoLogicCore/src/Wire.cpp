#include <Wire.h>

namespace TopoLogicCore
{
	void Wire::Edges(std::list<Edge*>& rEdges) const
	{
	}
	void Wire::Faces(std::list<Face*>& rFaces) const
	{
	}
	bool Wire::IsClosed() const
	{
		return false;
	}
	void Wire::Vertices(std::list<Vertex*>& rVertices) const
	{
	}
	Wire * Wire::ByEdges(const std::list<Edge*>& rkEdges)
	{
		return nullptr;
	}
	Wire * Wire::ByPolyCurve(TopoDS_Wire const * const kpkWire)
	{
		return nullptr;
	}
	Wire::Wire(TopoDS_Wire * const kpOcctWire)
		: Topology(1)
		, m_pOcctWire(kpOcctWire)
	{
	}
	Wire::~Wire()
	{
	}
}
