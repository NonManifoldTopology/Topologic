#include <Face.h>

namespace TopoLogicCore
{
	void Face::AdjacentFaces(std::list<Face*>& rFaces) const
	{
	}
	void Face::Apertures(std::list<Face*>& rFaces) const
	{
	}
	void Face::Cells(std::list<Cell*>& rCells) const
	{
	}
	void Face::Edges(std::list<Edge*>& rEdges) const
	{
	}
	bool Face::IsApplied() const
	{
		return false;
	}
	void Face::Shells(std::list<Shell*>& rShells) const
	{
	}
	void Face::Vertices(std::list<Vertex*>& rVertices) const
	{
	}
	void Face::Wires(std::list<Wire*>& rWires) const
	{
	}
	bool Face::ApplyApertures(const std::list<Face*>& rkApertures) const
	{
		return false;
	}
	Face * Face::ByClosedWire(Wire const * const kpkWire)
	{
		return nullptr;
	}
	Face * Face::ByEdges(const std::list<Edge*>* rkEdges)
	{
		return nullptr;
	}
	Face * Face::BySurface(Handle(Geom_Surface) pOcctSurface)
	{
		return nullptr;
	}
	void Face::SharedEdges(Face const * const kpkAnotherFace, std::list<Edge*>& rEdges) const
	{
	}
	void Face::SharedVertices(Face const * const kpkAnotherFace, std::list<Vertex*>& rVertices) const
	{
	}
	Face::Face(TopoDS_Face * const kpOcctFace)
		: Topology(2)
		, m_pOcctFace(kpOcctFace)
	{
	}
	Face::~Face()
	{
	}
}
