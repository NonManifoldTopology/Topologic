#include <DualGraph.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Face.h>
#include <Edge.h>
#include <Vertex.h>
#include <DualGraphFactory.h>
#include <Aperture.h>
#include <Context.h>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <StdFail_NotDone.hxx>

namespace TopologicCore
{
	DualGraph::Ptr DualGraph::ByCellComplex(
		const CellComplex::Ptr& kpCellComplex,
		const bool kUseCells,
		const bool kUseNonManifoldFaces,
		const bool kUseManifoldFaces,
		const bool kUseApertures)
	{
		if (!kUseCells)
		{
			return nullptr;
		}

		TopTools_ListOfShape occtEdges;
		if (kUseNonManifoldFaces)
		{
			// Get the non-manifold faces
			std::list<Face::Ptr> nonManifoldFaces;
			kpCellComplex->NonManifoldFaces(nonManifoldFaces);

			// For each non-manifold face,
			for (const Face::Ptr& kpNonManifoldFace : nonManifoldFaces)
			{
				// Get the adjacent cells
				std::list<Cell::Ptr> adjacentCells;
				kpNonManifoldFace->Cells(kpCellComplex, adjacentCells);

				// Connect the face centroid with the cell centroid
				Vertex::Ptr faceCentroid = kpNonManifoldFace->CenterOfMass();
				for (const Cell::Ptr& kpAdjacentCell : adjacentCells)
				{
					Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

					std::list<Vertex::Ptr> vertices;
					vertices.push_back(faceCentroid);
					vertices.push_back(cellCentroid);
					Edge::Ptr pEdge = Edge::ByVertices(vertices);
					occtEdges.Append(pEdge->GetOcctShape());
				}
			}
		}

		if (kUseManifoldFaces)
		{
			// Get the faces
			std::list<Face::Ptr> manifoldFaces;
			kpCellComplex->Faces(manifoldFaces);

			// For each face,
			for (const Face::Ptr& kpManifoldFace : manifoldFaces)
			{
				// If it is manifold, skip it.
				if (!kpManifoldFace->IsManifold(kpCellComplex.get()))
				{
					continue;
				}

				// Get the adjacent cells
				std::list<Cell::Ptr> adjacentCells;
				kpManifoldFace->Cells(kpCellComplex, adjacentCells);

				// Connect the face centroid with the cell centroid
				Vertex::Ptr faceCentroid = kpManifoldFace->CenterOfMass();
				for (const Cell::Ptr& kpAdjacentCell : adjacentCells)
				{
					Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

					std::list<Vertex::Ptr> vertices;
					vertices.push_back(faceCentroid);
					vertices.push_back(cellCentroid);
					Edge::Ptr pEdge = Edge::ByVertices(vertices);
					occtEdges.Append(pEdge->GetOcctShape());
				}
			}
		}

		if (kUseApertures)
		{
			// Get the apertures
			std::list<Topology::Ptr> contents;
			kpCellComplex->Contents(true, contents);

			for (const Topology::Ptr& kpContent : contents)
			{
				// If this is not an aperture, skip it
				if (kpContent->GetType() != TOPOLOGY_APERTURE)
				{
					continue;
				}
				Aperture::Ptr pAperture = TopologicalQuery::Downcast<Aperture>(kpContent);
				Topology::Ptr pApertureTopology = pAperture->Topology();
				
				// Connect the face centroid with the cell centroid
				Vertex::Ptr faceCentroid = pApertureTopology->CenterOfMass();
				
				// For now, do only faces which have faces as context
				std::list<Context::Ptr> contexts;
				pAperture->Contexts(contexts);
				if (pApertureTopology->GetType() == TOPOLOGY_FACE)
				{
					for (const Context::Ptr& kpContext : contexts)
					{
						Topology::Ptr pContextTopology = kpContext->Topology();
						if (pContextTopology->GetType() == TOPOLOGY_FACE)
						{
							Face::Ptr pFaceContextTopology = TopologicalQuery::Downcast<Face>(pContextTopology);
							// Get the adjacent cells of the context topology
							std::list<Cell::Ptr> adjacentCells;
							pFaceContextTopology->Cells(kpCellComplex, adjacentCells);

							for (const Cell::Ptr& kpAdjacentCell : adjacentCells)
							{
								Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

								std::list<Vertex::Ptr> vertices;
								vertices.push_back(faceCentroid);
								vertices.push_back(cellCentroid);
								Edge::Ptr pEdge = Edge::ByVertices(vertices);
								occtEdges.Append(pEdge->GetOcctShape());
							}
						}
					}
					
				}
			}
		}

		BRepBuilderAPI_MakeWire occtMakeWire;
		occtMakeWire.Add(occtEdges);

		if (occtEdges.IsEmpty())
		{
			return nullptr;
		}

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
		RegisterFactory(GetClassGUID(), std::make_shared<DualGraphFactory>());
	}

	DualGraph::~DualGraph()
	{

	}
	std::string DualGraph::GetTypeAsString() const
	{
		return std::string("DualGraph");
	}
}