#include <Face.h>

namespace TopoLogicCore
{
	void Face::AdjacentFaces(Face const * const kpkFace, std::list<Face*>& rFaces)
	{
	}
	void Face::Apertures(Face const * const kpkFace, std::list<Face*>& rFaces)
	{
	}
	void Face::Cells(Face const * const kpkFace, std::list<Cell*>& rCells)
	{
	}
	void Face::Edges(Face const * const kpkFace, std::list<Edge*>& rEdges)
	{
	}
	bool Face::IsApplied(Face const * const kpkFace)
	{
		return false;
	}
	void Face::Shells(Face const * const kpkFace, std::list<Shell*>& rShells)
	{
	}
	void Face::Vertices(Face const * const kpkFace, std::list<Vertex*>& rVertices)
	{
	}
	void Face::Wires(Face const * const kpkFace, std::list<Wire*>& rWires)
	{
	}
	bool Face::ApplyApertures(const std::list<Face*>& rkApertures)
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
	void Face::SharedEdges(Face const * const kpkFace1, Face const * const kpkFace2, std::list<Edge*>& rEdges)
	{
	}
	void Face::SharedVertices(Face const * const kpkFace1, Face const * const kpkFace2, std::list<Vertex*>& rVertices)
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
