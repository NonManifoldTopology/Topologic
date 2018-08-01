#include <DualGraph.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Face.h>
#include <Edge.h>
#include <Vertex.h>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <StdFail_NotDone.hxx>

namespace TopologicCore
{
	DualGraph::Ptr DualGraph::ByCellComplex(const CellComplex::Ptr& kpCellComplex)
	{
		// Get the non-manifold faces
		std::list<Face::Ptr> nonManifoldFaces;
		kpCellComplex->NonManifoldFaces(nonManifoldFaces);

		TopTools_ListOfShape occtEdges;
		// For each non-manifold face,
		for (const Face::Ptr& kpNonManifoldFace : nonManifoldFaces)
		{
			// Get the adjacent cells
			std::list<Cell::Ptr> adjacentCells;
			kpNonManifoldFace->Cells(kpCellComplex, adjacentCells);

			// Connect the face centroid with the cell centroid
			Vertex::Ptr faceCentroid = kpNonManifoldFace->CenterOfMass();
			for (const Cell::Ptr& kpAdjacentCell: adjacentCells)
			{
				Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

				std::list<Vertex::Ptr> vertices;
				vertices.push_back(faceCentroid);
				vertices.push_back(cellCentroid);
				Edge::Ptr pEdge = Edge::ByVertices(vertices);
				occtEdges.Append(pEdge->GetOcctShape());
			}
		}

		BRepBuilderAPI_MakeWire occtMakeWire;
		occtMakeWire.Add(occtEdges);

		try {
			DualGraph::Ptr pDualGraph = std::make_shared<DualGraph>(occtMakeWire);

			return pDualGraph;
		}
		catch (StdFail_NotDone&)
		{
			Throw(occtMakeWire);
			return nullptr;
		}
		return nullptr;
	}

	DualGraph::DualGraph(const TopoDS_Wire& rkOcctWire, const std::string& rkGuid)
		: Wire(rkOcctWire, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
	{

	}

	DualGraph::~DualGraph()
	{

	}
	std::string DualGraph::GetTypeAsString() const
	{
		return std::string("DualGraph");
	}
}