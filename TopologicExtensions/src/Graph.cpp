//#include "Graph.h"
//#include "GraphFactory.h"
//
//#include <TopologicCore/include/CellComplex.h>
//#include <TopologicCore/include/Cell.h>
//#include <TopologicCore/include/Face.h>
//#include <TopologicCore/include/Edge.h>
//#include <TopologicCore/include/Shell.h>
//#include <TopologicCore/include/Vertex.h>
//#include <TopologicCore/include/Wire.h>
//#include <TopologicCore/include/Aperture.h>
//#include <TopologicCore/include/Context.h>
//
//#include <BRepBuilderAPI_MakeWire.hxx>
//#include <StdFail_NotDone.hxx>
//
//namespace TopologicExtensions
//{
//	DualGraph_::Ptr DualGraph_::ByCellComplex(const std::shared_ptr<TopologicCore::CellComplex>& kpCellComplex, const bool kDirect, const bool kViaSharedFaces, const bool kViaSharedApertures, const bool kToExteriorFaces, const bool kToExteriorApertures)
//	{
//		if (kpCellComplex == nullptr)
//		{
//			return nullptr;
//		}
//
//		// 1. Get the vertices mapped to their original topologies
//		//    - Cell --> centroid
//		//   Occt shapes must be used as the keys. Topologic shapes cannot be used because there can be many shapes representing the same OCCT shapes.
//		std::map<TopoDS_Solid, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> cellCentroids;
//
//		std::list<TopologicCore::Cell::Ptr> cells;
//		kpCellComplex->Cells(cells);
//		for (const TopologicCore::Cell::Ptr& kpCell : cells)
//		{
//			TopologicCore::Vertex::Ptr pCentroid = kpCell->CenterOfMass();
//			cellCentroids.insert(std::make_pair(kpCell->GetOcctSolid(), pCentroid));
//		}
//
//		// 2. Check the configurations. Add the edges to a cluster.
//		std::list<TopologicCore::Topology::Ptr> edges;
//		if (kDirect)
//		{
//			// Iterate through all cells and check for adjacency.
//			// For each adjacent cells, connect the centroids
//			for (const TopologicCore::Cell::Ptr& kpCell : cells)
//			{
//				std::list<TopologicCore::Cell::Ptr> adjacentCells;
//				kpCell->AdjacentCells(adjacentCells);
//
//				for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
//				{
//					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(cellCentroids[kpCell->GetOcctSolid()], cellCentroids[kpAdjacentCell->GetOcctSolid()]);
//					edges.push_back(pEdge);
//				}
//			}
//		}
//
//		std::list<TopologicCore::Face::Ptr> faces;
//		kpCellComplex->Faces(faces);
//		for (const TopologicCore::Face::Ptr& kpFace : faces)
//		{
//			TopologicCore::Vertex::Ptr pCentroid = kpFace->CenterOfMass();
//			bool isManifold = kpFace->IsManifold();
//			std::list<TopologicCore::Cell::Ptr> adjacentCells;
//			kpFace->Cells(adjacentCells);
//
//			std::list<Topology::Ptr> contents;
//			kpFace->Contents(contents);
//
//			// Get the apertures and calculate their centroids
//			//std::map<TopoDS_Shape, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> apertureCentroids;
//			std::list<TopologicCore::Vertex::Ptr> apertureCentroids;
//			for (const Topology::Ptr& kpContent : contents)
//			{
//				// If this is not an aperture, skip it
//				if (kpContent->GetType() != TopologicCore::TOPOLOGY_APERTURE)
//				{
//					continue;
//				}
//				TopologicCore::Aperture::Ptr pAperture = TopologicalQuery::Downcast<TopologicCore::Aperture>(kpContent);
//				Topology::Ptr pApertureTopology = pAperture->Topology();
//				TopologicCore::Vertex::Ptr pApertureCentroid = pApertureTopology->CenterOfMass();
//
//				apertureCentroids.push_back(pApertureCentroid);
//			}
//
//			// Check 
//			for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
//			{
//				if ((!isManifold && kViaSharedFaces) // i.e. non-manifold faces
//					||
//					(isManifold && kToExteriorFaces))
//				{
//					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(pCentroid, cellCentroids[kpAdjacentCell->GetOcctSolid()]);
//					edges.push_back(pEdge);
//				}
//
//				for (const TopologicCore::Vertex::Ptr& rkApertureCentroid : apertureCentroids)
//				{
//					if ((!isManifold && kViaSharedApertures) // i.e. non-manifold apertures
//						||
//						(isManifold && kToExteriorApertures))
//					{
//						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(rkApertureCentroid, cellCentroids[kpAdjacentCell->GetOcctSolid()]);
//						edges.push_back(pEdge);
//					}
//				}
//			}
//		}
//
//		// 3. Self-merge the cluster
//		if (edges.empty())
//		{
//			return nullptr;
//		}
//
//		TopologicCore::Cluster::Ptr pTopologiesAsCluster = TopologicCore::Cluster::ByTopologies(edges);
//		TopologicCore::Topology::Ptr pSelfMergedTopologies = pTopologiesAsCluster->SelfMerge();
//		
//		// If this is a cluster, use it to create a dual graph. Otherwise, add it to a cluster to be used to create a dual graph.
//		TopologicCore::Cluster::Ptr pSelfMergedTopologiesAsCluster = std::dynamic_pointer_cast<Cluster>(pSelfMergedTopologies);
//		if (pSelfMergedTopologiesAsCluster != nullptr)
//		{
//			return std::make_shared<DualGraph_>(pSelfMergedTopologiesAsCluster->GetOcctCompound());
//		}
//		else
//		{
//			std::list<TopologicCore::Topology::Ptr> topologies;
//			topologies.push_back(pSelfMergedTopologies);
//			TopologicCore::Cluster::Ptr pDualGraphAsCluster = TopologicCore::Cluster::ByTopologies(topologies);
//			return std::make_shared<DualGraph_>(pDualGraphAsCluster->GetOcctCompound());
//		}
//	}
//
//	DualGraph_::Ptr DualGraph_::ByShell(const std::shared_ptr<TopologicCore::Shell>& kpShell, const bool kDirect, const bool kViaSharedEdges, const bool kViaSharedApertures, const bool kToExteriorEdges, const bool kToExteriorApertures)
//	{
//		if (kpShell == nullptr)
//		{
//			return nullptr;
//		}
//
//		// 1. Get the vertices mapped to their original topologies
//		//    - Face --> centroid
//		//   Occt shapes must be used as the keys. Topologic shapes cannot be used because there can be many shapes representing the same OCCT shapes.
//		std::map<TopoDS_Face, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> faceCentroids;
//
//		std::list<TopologicCore::Face::Ptr> faces;
//		kpShell->Faces(faces);
//		for (const TopologicCore::Face::Ptr& kpFace : faces)
//		{
//			TopologicCore::Vertex::Ptr pCentroid = kpFace->CenterOfMass();
//			faceCentroids.insert(std::make_pair(kpFace->GetOcctFace(), pCentroid));
//		}
//
//		// 2. Check the configurations. Add the edges to a cluster.
//		std::list<TopologicCore::Topology::Ptr> graphEdges;
//		if (kDirect)
//		{
//			// Iterate through all faces and check for adjacency.
//			// For each adjacent faces, connect the centroids
//			for (const TopologicCore::Face::Ptr& kpFace : faces)
//			{
//				std::list<TopologicCore::Face::Ptr> adjacentFaces;
//				kpFace->AdjacentFaces(adjacentFaces);
//
//				for (const TopologicCore::Face::Ptr& kpAdjacentFace : adjacentFaces)
//				{
//					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(faceCentroids[kpFace->GetOcctFace()], faceCentroids[kpAdjacentFace->GetOcctFace()]);
//					graphEdges.push_back(pEdge);
//				}
//			}
//		}
//
//		std::list<TopologicCore::Edge::Ptr> edges;
//		kpShell->Edges(edges);
//		for (const TopologicCore::Edge::Ptr& kpEdge : edges)
//		{
//			TopologicCore::Vertex::Ptr pCentroid = kpEdge->CenterOfMass();
//			bool isManifold = kpEdge->IsManifold();
//			std::list<TopologicCore::Face::Ptr> adjacentFaces;
//			kpEdge->Faces(adjacentFaces);
//			
//			std::list<Topology::Ptr> contents;
//			kpEdge->Contents(contents);
//
//			// Get the apertures and calculate their centroids
//			//std::map<TopoDS_Shape, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> apertureCentroids;
//			std::list<TopologicCore::Vertex::Ptr> apertureCentroids;
//			for (const Topology::Ptr& kpContent : contents)
//			{
//				// If this is not an aperture, skip it
//				if (kpContent->GetType() != TopologicCore::TOPOLOGY_APERTURE)
//				{
//					continue;
//				}
//				TopologicCore::Aperture::Ptr pAperture = TopologicalQuery::Downcast<TopologicCore::Aperture>(kpContent);
//				Topology::Ptr pApertureTopology = pAperture->Topology();
//				TopologicCore::Vertex::Ptr pApertureCentroid = pApertureTopology->CenterOfMass();
//
//				apertureCentroids.push_back(pApertureCentroid);
//			}
//
//			// Check 
//			for (const TopologicCore::Face::Ptr& kpAdjacentFace : adjacentFaces)
//			{
//				if ((!isManifold && kViaSharedEdges) // i.e. non-manifold faces
//					||
//					(isManifold && kToExteriorEdges))
//				{
//					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(pCentroid, faceCentroids[kpAdjacentFace->GetOcctFace()]);
//					graphEdges.push_back(pEdge);
//				}
//
//				for (const TopologicCore::Vertex::Ptr& rkApertureCentroid : apertureCentroids)
//				{
//					if ((!isManifold && kViaSharedApertures) // i.e. non-manifold apertures
//						||
//						(isManifold && kToExteriorApertures))
//					{
//						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(rkApertureCentroid, faceCentroids[kpAdjacentFace->GetOcctFace()]);
//						graphEdges.push_back(pEdge);
//					}
//				}
//			}
//		}
//
//		// 3. Self-merge the cluster
//		if (graphEdges.empty())
//		{
//			return nullptr;
//		}
//
//		TopologicCore::Cluster::Ptr pTopologiesAsCluster = TopologicCore::Cluster::ByTopologies(graphEdges);
//		TopologicCore::Topology::Ptr pSelfMergedTopologies = pTopologiesAsCluster->SelfMerge();
//
//		// If this is a cluster, use it to create a dual graph. Otherwise, add it to a cluster to be used to create a dual graph.
//		TopologicCore::Cluster::Ptr pSelfMergedTopologiesAsCluster = std::dynamic_pointer_cast<Cluster>(pSelfMergedTopologies);
//		if (pSelfMergedTopologiesAsCluster != nullptr)
//		{
//			return std::make_shared<DualGraph_>(pSelfMergedTopologiesAsCluster->GetOcctCompound());
//		}
//		else
//		{
//			std::list<TopologicCore::Topology::Ptr> topologies;
//			topologies.push_back(pSelfMergedTopologies);
//			TopologicCore::Cluster::Ptr pDualGraphAsCluster = TopologicCore::Cluster::ByTopologies(topologies);
//			return std::make_shared<DualGraph_>(pDualGraphAsCluster->GetOcctCompound());
//		}
//	}
//
//	DualGraph_::DualGraph_(const TopoDS_Compound& rkOcctCompound, const std::string& rkGuid)
//		: Cluster(rkOcctCompound, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
//	{
//		RegisterFactory(GetClassGUID(), std::make_shared<GraphFactory>());
//	}
//
//	DualGraph_::~DualGraph_()
//	{
//
//	}
//
//	std::string DualGraph_::GetTypeAsString() const
//	{
//		return std::string("Graph");
//	}
//}