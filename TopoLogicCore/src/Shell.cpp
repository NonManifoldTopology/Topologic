#include <Shell.h>

namespace TopoLogicCore
{
	Cell * Shell::Cell(Shell const * const kpkShell)
	{
		return nullptr;
	}
	void Shell::Edges(Shell const * const kpkShell, std::list<Edge*>& rEdges)
	{
	}
	void Shell::Faces(Shell const * const kpkShell, std::list<Face*>& rFaces)
	{
	}
	bool Shell::IsClosed(Shell const * const kpkShell)
	{
		return false;
	}
	void Shell::Vertices(Shell const * const kpkShell, std::list<Vertex*>& rVertices)
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
	Shell::Shell(TopoDS_Shell * const kpOcctShell)
		: Topology(2)
		, m_pOcctShell(kpOcctShell)
	{
	}
	Shell::~Shell()
	{
	}
}
