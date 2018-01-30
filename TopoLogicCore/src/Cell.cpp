#include <Cell.h>

namespace TopoLogicCore
{
	void Cell::AdjacentCells(std::list<Cell*>& rcells) const
	{
	}
	CellComplex * Cell::CellComplex() const
	{
		return nullptr;
	}
	void Cell::Edges(std::list<Edge*>& rEdges) const
	{
	}
	void Cell::Faces(std::list<Face*>& rFaces) const
	{
	}
	void Cell::Vertices(std::list<Vertex*>& rVertices) const
	{
	}
	void Cell::Wires(std::list<Wire*>& rWires) const
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
	void Cell::SharedEdges(Cell const * const kpkAnotherCell, std::list<Edge*>& rEdges) const
	{
	}
	void Cell::SharedFaces(Cell const * const kpkAnotherCell, std::list<Face*>& rFaces) const
	{
	}
	void Cell::SharedVertices(Cell const * const kpkAnotherCell, std::list<Vertex*>& rVertices) const
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
