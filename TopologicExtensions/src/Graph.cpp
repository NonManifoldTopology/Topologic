#include "Graph.h"
#include "GraphFactory.h"

#include <TopologicCore/include/CellComplex.h>
#include <TopologicCore/include/Cell.h>
#include <TopologicCore/include/Face.h>
#include <TopologicCore/include/Edge.h>
#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Wire.h>
#include <TopologicCore/include/Aperture.h>
#include <TopologicCore/include/Context.h>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <StdFail_NotDone.hxx>

namespace TopologicExtensions
{
	DualGraph_::Ptr DualGraph_::ByCellComplex(
		const TopologicCore::CellComplex::Ptr& kpCellComplex,
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
		if (kUseCells && !kUseNonManifoldFaces && !kUseManifoldFaces)
		{
			// Connect the centroids of adjacent cells
			// Get the non-manifold faces
			std::list<TopologicCore::Face::Ptr> nonManifoldFaces;
			kpCellComplex->NonManifoldFaces(nonManifoldFaces);

			// For each non-manifold face,
			for (const TopologicCore::Face::Ptr& kpNonManifoldFace : nonManifoldFaces)
			{
				// Get the adjacent cells
				std::list<TopologicCore::Cell::Ptr> adjacentCells;
				kpNonManifoldFace->Cells(adjacentCells);

				std::list<TopologicCore::Vertex::Ptr> centroids;
				for (const TopologicCore::Cell::Ptr& kpCell : adjacentCells)
				{
					centroids.push_back(kpCell->CenterOfMass());
				}

				if (centroids.size() < 2)
				{
					return nullptr;
				}

				std::list<TopologicCore::Vertex::Ptr>::iterator endCentroid = centroids.end();
				for (std::list<TopologicCore::Vertex::Ptr>::iterator centroidIterator = centroids.begin();
					centroidIterator != endCentroid;
					centroidIterator++)
				{
					std::list<TopologicCore::Vertex::Ptr>::iterator nextIterator = centroidIterator;
					nextIterator++;

					for (std::list<TopologicCore::Vertex::Ptr>::iterator nextCentroidIterator = nextIterator;
						nextCentroidIterator != endCentroid;
						nextCentroidIterator++)
					{
						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(*centroidIterator, *nextCentroidIterator);
						occtEdges.Append(pEdge->GetOcctShape());
					}
				}
			}


			/*std::list<Cell::Ptr> cells;
			kpCellComplex->Cells(cells);

			std::list<Vertex::Ptr> centroids;
			for (const Cell::Ptr& kpCell : cells)
			{
				centroids.push_back(kpCell->CenterOfMass());
			}

			if (centroids.size() < 2)
			{
				return nullptr;
			}

			std::list<Vertex::Ptr>::iterator endCentroid = centroids.end();
			for (std::list<Vertex::Ptr>::iterator centroidIterator = centroids.begin();
				centroidIterator != endCentroid;
				centroidIterator++)
			{
				std::list<Vertex::Ptr>::iterator nextIterator = centroidIterator;
				nextIterator++;
				
				for (std::list<Vertex::Ptr>::iterator nextCentroidIterator = nextIterator;
					nextCentroidIterator != endCentroid;
					nextCentroidIterator++)
				{
					std::list<Vertex::Ptr> vertices;
					vertices.push_back(*centroidIterator);
					vertices.push_back(*nextCentroidIterator);
					Edge::Ptr pEdge = Edge::ByVertices(vertices);
					occtEdges.Append(pEdge->GetOcctShape());
				}
			}*/
		}
		else
		{

			if (kUseNonManifoldFaces)
			{
				// Get the non-manifold faces
				std::list<TopologicCore::Face::Ptr> nonManifoldFaces;
				kpCellComplex->NonManifoldFaces(nonManifoldFaces);

				// For each non-manifold face,
				for (const TopologicCore::Face::Ptr& kpNonManifoldFace : nonManifoldFaces)
				{
					// Get the adjacent cells
					std::list<TopologicCore::Cell::Ptr> adjacentCells;
					kpNonManifoldFace->Cells(adjacentCells);

					// Connect the face centroid with the cell centroid
					TopologicCore::Vertex::Ptr faceCentroid = kpNonManifoldFace->CenterOfMass();
					for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
					{
						TopologicCore::Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(faceCentroid, cellCentroid);
						occtEdges.Append(pEdge->GetOcctShape());
					}
				}
			}

			if (kUseManifoldFaces)
			{
				// Get the faces
				std::list<TopologicCore::Face::Ptr> manifoldFaces;
				kpCellComplex->Faces(manifoldFaces);

				// For each face,
				for (const TopologicCore::Face::Ptr& kpManifoldFace : manifoldFaces)
				{
					// If it is manifold, skip it.
					if (!kpManifoldFace->IsManifold())
					{
						continue;
					}

					// Get the adjacent cells
					std::list<TopologicCore::Cell::Ptr> adjacentCells;
					kpManifoldFace->Cells(adjacentCells);

					// Connect the face centroid with the cell centroid
					TopologicCore::Vertex::Ptr faceCentroid = kpManifoldFace->CenterOfMass();
					for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
					{
						TopologicCore::Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(faceCentroid, cellCentroid);
						occtEdges.Append(pEdge->GetOcctShape());
					}
				}
			}
		}

		if (kUseApertures)
		{
			// Get the apertures
			std::list<Topology::Ptr> contents;
			kpCellComplex->SubContents(contents);

			for (const Topology::Ptr& kpContent : contents)
			{
				// If this is not an aperture, skip it
				if (kpContent->GetType() != TopologicCore::TOPOLOGY_APERTURE)
				{
					continue;
				}
				TopologicCore::Aperture::Ptr pAperture = TopologicalQuery::Downcast<TopologicCore::Aperture>(kpContent);
				Topology::Ptr pApertureTopology = pAperture->Topology();
				
				// Connect the face centroid with the cell centroid
				TopologicCore::Vertex::Ptr faceCentroid = pApertureTopology->CenterOfMass();
				
				// For now, do only faces which have faces as context
				std::list<TopologicCore::Context::Ptr> contexts;
				pAperture->Contexts(contexts);
				if (pApertureTopology->GetType() == TopologicCore::TOPOLOGY_FACE)
				{
					for (const TopologicCore::Context::Ptr& kpContext : contexts)
					{
						Topology::Ptr pContextTopology = kpContext->Topology();
						if (pContextTopology->GetType() == TopologicCore::TOPOLOGY_FACE)
						{
							TopologicCore::Face::Ptr pFaceContextTopology = TopologicalQuery::Downcast<TopologicCore::Face>(pContextTopology);
							// Get the adjacent cells of the context topology
							std::list<TopologicCore::Cell::Ptr> adjacentCells;
							pFaceContextTopology->Cells(adjacentCells);

							for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
							{
								TopologicCore::Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

								TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(faceCentroid, cellCentroid);
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
			TopologicCore::Wire::Ptr pCoreWire = std::make_shared<TopologicCore::Wire>(occtMakeWire);
			TopoDS_Compound occtCompound;
			TopoDS_Builder occtBuilder;
			occtBuilder.MakeCompound(occtCompound);
			DualGraph_::Ptr pGraph = std::make_shared<DualGraph_>(occtCompound);
			pGraph->AddTopology(pCoreWire.get());
			return pGraph;
		}
		catch (StdFail_NotDone&)
		{
			TopologicCore::Wire::Throw(occtMakeWire);
			return nullptr;
		}
		return nullptr;
	}

	DualGraph_::DualGraph_(const TopoDS_Compound& rkOcctCompound, const std::string& rkGuid)
		: Cluster(rkOcctCompound, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<GraphFactory>());
	}

	DualGraph_::~DualGraph_()
	{

	}
	std::string DualGraph_::GetTypeAsString() const
	{
		return std::string("Graph");
	}
}