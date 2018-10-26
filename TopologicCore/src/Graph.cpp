#include <Graph.h>
#include <CellComplex.h>
#include <Cell.h>
#include <Face.h>
#include <Edge.h>
#include <Vertex.h>
#include <GraphFactory.h>
#include <Aperture.h>
#include <Context.h>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <StdFail_NotDone.hxx>

namespace TopologicCore
{
	Graph::Ptr Graph::ByCellComplex(
		const CellComplex::Ptr& kpCellComplex,
		const bool kUseCells,
		const bool kUseNonManifoldFaces,
		const bool kUseManifoldFaces,
		const bool kUseApertures)
	{
		//if (!kUseCells)
		//{
		//	return nullptr;
		//}

		//TopTools_ListOfShape occtEdges;
		//if (kUseCells && !kUseNonManifoldFaces && !kUseManifoldFaces)
		//{
		//	// Connect the centroids of adjacent cells
		//	// Get the non-manifold faces
		//	std::list<Face::Ptr> nonManifoldFaces;
		//	kpCellComplex->NonManifoldFaces(nonManifoldFaces);

		//	// For each non-manifold face,
		//	for (const Face::Ptr& kpNonManifoldFace : nonManifoldFaces)
		//	{
		//		// Get the adjacent cells
		//		std::list<Cell::Ptr> adjacentCells;
		//		kpNonManifoldFace->Cells(adjacentCells);

		//		std::list<Vertex::Ptr> centroids;
		//		for (const Cell::Ptr& kpCell : adjacentCells)
		//		{
		//			centroids.push_back(kpCell->CenterOfMass());
		//		}

		//		if (centroids.size() < 2)
		//		{
		//			return nullptr;
		//		}

		//		std::list<Vertex::Ptr>::iterator endCentroid = centroids.end();
		//		for (std::list<Vertex::Ptr>::iterator centroidIterator = centroids.begin();
		//			centroidIterator != endCentroid;
		//			centroidIterator++)
		//		{
		//			std::list<Vertex::Ptr>::iterator nextIterator = centroidIterator;
		//			nextIterator++;

		//			for (std::list<Vertex::Ptr>::iterator nextCentroidIterator = nextIterator;
		//				nextCentroidIterator != endCentroid;
		//				nextCentroidIterator++)
		//			{
		//				std::list<Vertex::Ptr> vertices;
		//				vertices.push_back(*centroidIterator);
		//				vertices.push_back(*nextCentroidIterator);
		//				Edge::Ptr pEdge = Edge::ByVertices(vertices);
		//				occtEdges.Append(pEdge->GetOcctShape());
		//			}
		//		}
		//	}


		//	/*std::list<Cell::Ptr> cells;
		//	kpCellComplex->Cells(cells);

		//	std::list<Vertex::Ptr> centroids;
		//	for (const Cell::Ptr& kpCell : cells)
		//	{
		//		centroids.push_back(kpCell->CenterOfMass());
		//	}

		//	if (centroids.size() < 2)
		//	{
		//		return nullptr;
		//	}

		//	std::list<Vertex::Ptr>::iterator endCentroid = centroids.end();
		//	for (std::list<Vertex::Ptr>::iterator centroidIterator = centroids.begin();
		//		centroidIterator != endCentroid;
		//		centroidIterator++)
		//	{
		//		std::list<Vertex::Ptr>::iterator nextIterator = centroidIterator;
		//		nextIterator++;
		//		
		//		for (std::list<Vertex::Ptr>::iterator nextCentroidIterator = nextIterator;
		//			nextCentroidIterator != endCentroid;
		//			nextCentroidIterator++)
		//		{
		//			std::list<Vertex::Ptr> vertices;
		//			vertices.push_back(*centroidIterator);
		//			vertices.push_back(*nextCentroidIterator);
		//			Edge::Ptr pEdge = Edge::ByVertices(vertices);
		//			occtEdges.Append(pEdge->GetOcctShape());
		//		}
		//	}*/
		//}
		//else
		//{

		//	if (kUseNonManifoldFaces)
		//	{
		//		// Get the non-manifold faces
		//		std::list<Face::Ptr> nonManifoldFaces;
		//		kpCellComplex->NonManifoldFaces(nonManifoldFaces);

		//		// For each non-manifold face,
		//		for (const Face::Ptr& kpNonManifoldFace : nonManifoldFaces)
		//		{
		//			// Get the adjacent cells
		//			std::list<Cell::Ptr> adjacentCells;
		//			kpNonManifoldFace->Cells(adjacentCells);

		//			// Connect the face centroid with the cell centroid
		//			Vertex::Ptr faceCentroid = kpNonManifoldFace->CenterOfMass();
		//			for (const Cell::Ptr& kpAdjacentCell : adjacentCells)
		//			{
		//				Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

		//				std::list<Vertex::Ptr> vertices;
		//				vertices.push_back(faceCentroid);
		//				vertices.push_back(cellCentroid);
		//				Edge::Ptr pEdge = Edge::ByVertices(vertices);
		//				occtEdges.Append(pEdge->GetOcctShape());
		//			}
		//		}
		//	}

		//	if (kUseManifoldFaces)
		//	{
		//		// Get the faces
		//		std::list<Face::Ptr> manifoldFaces;
		//		kpCellComplex->Faces(manifoldFaces);

		//		// For each face,
		//		for (const Face::Ptr& kpManifoldFace : manifoldFaces)
		//		{
		//			// If it is manifold, skip it.
		//			if (!kpManifoldFace->IsManifold())
		//			{
		//				continue;
		//			}

		//			// Get the adjacent cells
		//			std::list<Cell::Ptr> adjacentCells;
		//			kpManifoldFace->Cells(adjacentCells);

		//			// Connect the face centroid with the cell centroid
		//			Vertex::Ptr faceCentroid = kpManifoldFace->CenterOfMass();
		//			for (const Cell::Ptr& kpAdjacentCell : adjacentCells)
		//			{
		//				Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

		//				std::list<Vertex::Ptr> vertices;
		//				vertices.push_back(faceCentroid);
		//				vertices.push_back(cellCentroid);
		//				Edge::Ptr pEdge = Edge::ByVertices(vertices);
		//				occtEdges.Append(pEdge->GetOcctShape());
		//			}
		//		}
		//	}
		//}

		//if (kUseApertures)
		//{
		//	// Get the apertures
		//	std::list<Topology::Ptr> contents;
		//	kpCellComplex->Contents(true, contents);

		//	for (const Topology::Ptr& kpContent : contents)
		//	{
		//		// If this is not an aperture, skip it
		//		if (kpContent->GetType() != TOPOLOGY_APERTURE)
		//		{
		//			continue;
		//		}
		//		Aperture::Ptr pAperture = TopologicalQuery::Downcast<Aperture>(kpContent);
		//		Topology::Ptr pApertureTopology = pAperture->Topology();
		//		
		//		// Connect the face centroid with the cell centroid
		//		Vertex::Ptr faceCentroid = pApertureTopology->CenterOfMass();
		//		
		//		// For now, do only faces which have faces as context
		//		std::list<Context::Ptr> contexts;
		//		pAperture->Contexts(contexts);
		//		if (pApertureTopology->GetType() == TOPOLOGY_FACE)
		//		{
		//			for (const Context::Ptr& kpContext : contexts)
		//			{
		//				Topology::Ptr pContextTopology = kpContext->Topology();
		//				if (pContextTopology->GetType() == TOPOLOGY_FACE)
		//				{
		//					Face::Ptr pFaceContextTopology = TopologicalQuery::Downcast<Face>(pContextTopology);
		//					// Get the adjacent cells of the context topology
		//					std::list<Cell::Ptr> adjacentCells;
		//					pFaceContextTopology->Cells(adjacentCells);

		//					for (const Cell::Ptr& kpAdjacentCell : adjacentCells)
		//					{
		//						Vertex::Ptr cellCentroid = kpAdjacentCell->CenterOfMass();

		//						std::list<Vertex::Ptr> vertices;
		//						vertices.push_back(faceCentroid);
		//						vertices.push_back(cellCentroid);
		//						Edge::Ptr pEdge = Edge::ByVertices(vertices);
		//						occtEdges.Append(pEdge->GetOcctShape());
		//					}
		//				}
		//			}
		//			
		//		}
		//	}
		//}

		//BRepBuilderAPI_MakeWire occtMakeWire;
		//occtMakeWire.Add(occtEdges);

		//if (occtEdges.IsEmpty())
		//{
		//	return nullptr;
		//}

		//try {
		//	Graph::Ptr pGraph = std::make_shared<Graph>(occtMakeWire);

		//	return pGraph;
		//}
		//catch (StdFail_NotDone&)
		//{
		//	Throw(occtMakeWire);
		//	return nullptr;
		//}
		return nullptr;
	}

	Graph::Graph(const TopoDS_Wire& rkOcctWire, const std::string& rkGuid)
		: Wire(rkOcctWire, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<GraphFactory>());
	}

	Graph::~Graph()
	{

	}
	std::string Graph::GetTypeAsString() const
	{
		return std::string("Graph");
	}
}