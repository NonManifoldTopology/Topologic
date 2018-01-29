#include <Cell.h>

namespace TopoLogicCore
{
	void Cell::AdjacentCells(Cell const * const kpkCell, std::list<Cell*>& rcells)
	{
	}
	CellComplex * Cell::CellComplex(Cell const * const kpkCell)
	{
		return nullptr;
	}
	void Cell::Edges(Cell const * const kpkCell, std::list<Edge*>& rEdges)
	{
	}
	void Cell::Faces(Cell const * const kpkCell, std::list<Face*>& rFaces)
	{
	}
	void Cell::Vertices(Cell const * const kpkCell, std::list<Vertex*>& rVertices)
	{
	}
	void Cell::Wires(Cell const * const kpkCell, std::list<Wire*>& rWires)
	{
	}
	Cell * Cell::BySolid(TopoDS_Solid const * const kpkOcctCell)
	{
		return nullptr;
	}
	Cell * Cell::ByFace(const std::list<Face*>& rkFaces)
	{
		return nullptr;
	}
	Cell * Cell::ByShell(Shell const * const kpkShells)
	{
		return nullptr;
	}
	Cell * Cell::ByVerticesFaceIndices(const std::list<Vertex*>& rkVertices, const std::list<int>& rkFaceIndices)
	{
		return nullptr;
	}
	void Cell::SharedEdges(Cell const * const kpkCell1, Cell const * const kpkCell2, std::list<Edge*>& rEdges)
	{
	}
	void Cell::SharedFaces(Cell const * const kpkCell1, Cell const * const kpkCell2, std::list<Face*>& rFaces)
	{
	}
	void Cell::SharedVertices(Cell const * const kpkCell1, Cell const * const kpkCell2, std::list<Vertex*>& rVertices)
	{
	}
	Cell::Cell(TopoDS_Solid * const kpOcctSolid)
		: Topology(3)
		, m_pOcctSolid(kpOcctSolid)
	{
	}
	Cell::~Cell()
	{
	}
}
