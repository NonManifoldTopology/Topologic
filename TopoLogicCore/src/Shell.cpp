#include <Shell.h>

namespace TopoLogicCore
{
	Cell * Shell::Cell() const
	{
		return nullptr;
	}
	void Shell::Edges(std::list<Edge*>& rEdges) const
	{
	}
	void Shell::Faces(std::list<Face*>& rFaces) const
	{
	}
	bool Shell::IsClosed() const
	{
		return false;
	}
	void Shell::Vertices(std::list<Vertex*>& rVertices) const
	{
	}
	Shell * Shell::ByFaces(const std::list<Face*>& rkFaces)
	{
		return nullptr;
	}
	Shell * Shell::ByPolySurface(TopoDS_Shell const * const kpkOcctShell)
	{
		return nullptr;
	}
	Shell * Shell::ByVerticesFaceIndices(const std::list<Vertex*>& rkVertices, const std::list<int>& rkFaceIndices)
	{
		return nullptr;
	}
	void Shell::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
	}
	Shell::Shell(TopoDS_Shell * const kpOcctShell)
		: Topology(2)
		, m_pOcctShell(kpOcctShell)
	{
	}
	Shell::~Shell()
	{
	}
}
