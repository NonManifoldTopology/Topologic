#include <Wire.h>

namespace TopoLogicCore
{
	void Wire::Edges(Wire const * const kpkWire, std::list<Edge*>& rEdges)
	{
	}
	void Wire::Faces(Wire const * const kpkWire, std::list<Face*>& rFaces)
	{
	}
	bool Wire::IsClosed(Wire const * const kpkWire)
	{
		return false;
	}
	void Wire::Vertices(Wire const * const kpkWire, std::list<Vertex*>& rVertices)
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
