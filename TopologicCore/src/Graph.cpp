#include "Graph.h"
#include "Cluster.h"
#include "Wire.h"
#include "Face.h"
#include "Shell.h"
#include "Cell.h"
#include "CellComplex.h"
#include "Aperture.h"

#include <BRepExtrema_DistShapeShape.hxx>

namespace TopologicCore
{
	Graph::Ptr Graph::ByVerticesEdges(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges)
	{
		return std::make_shared<Graph>(rkVertices, rkEdges);
	}

	Graph::Ptr Graph::ByTopology(
		const Topology::Ptr topology, 
		const bool kDirect,
		const bool kViaSharedTopologies, 
		const bool kViaSharedApertures, 
		const bool kToExteriorTopologies, 
		const bool kToExteriorApertures)
	{
		switch (topology->GetType())
		{
		case TOPOLOGY_VERTEX: return Graph::ByVertex(std::dynamic_pointer_cast<Vertex>(topology), kToExteriorApertures);
		case TOPOLOGY_EDGE: return Graph::ByEdge(std::dynamic_pointer_cast<Edge>(topology), kDirect, kToExteriorApertures);
		case TOPOLOGY_WIRE: return Graph::ByWire(std::dynamic_pointer_cast<Wire>(topology), kDirect, kToExteriorApertures);
		case TOPOLOGY_FACE: return Graph::ByFace(std::dynamic_pointer_cast<Face>(topology), kToExteriorTopologies, kToExteriorApertures);
		case TOPOLOGY_SHELL: return Graph::ByShell(
			std::dynamic_pointer_cast<Shell>(topology),
			kDirect,
			kViaSharedTopologies,
			kViaSharedApertures,
			kToExteriorTopologies,
			kToExteriorApertures);
		case TOPOLOGY_CELL: return Graph::ByCell(std::dynamic_pointer_cast<Cell>(topology), kToExteriorTopologies, kToExteriorApertures);
		case TOPOLOGY_CELLCOMPLEX: return Graph::ByCellComplex(std::dynamic_pointer_cast<CellComplex>(topology),
			kDirect,
			kViaSharedTopologies,
			kViaSharedApertures,
			kToExteriorTopologies,
			kToExteriorApertures);
		case TOPOLOGY_CLUSTER: return Graph::ByCluster(std::dynamic_pointer_cast<Cluster>(topology),
			kDirect,
			kViaSharedTopologies,
			kViaSharedApertures,
			kToExteriorTopologies,
			kToExteriorApertures);

		case TOPOLOGY_APERTURE: 
			return Graph::ByTopology(
				std::dynamic_pointer_cast<Aperture>(topology)->Topology(),
				kDirect,
				kViaSharedTopologies,
				kViaSharedApertures,
				kToExteriorTopologies,
				kToExteriorApertures);
		default:
			throw std::exception("Fails to create a graph due to an unknown type of topology.");
		}
	}

	Graph::Graph(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges)
		: m_vertices(rkVertices)
		, m_edges(rkEdges)
	{
		// 1. Add the vertices
		for (const Vertex::Ptr& kpVertex : rkVertices)
		{
			const TopoDS_Vertex& rkOcctVertex = kpVertex->GetOcctVertex();

			// a. Geometric check of the edges
			bool isAdded = false;
			for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& kpPair : m_graphDictionary)
			{
				const TopoDS_Vertex& rkOcctExistingVertex = kpPair.first;
				bool isAdded = AreVerticesGeometricallyIdentical(rkOcctVertex, rkOcctExistingVertex, 0.0001);
				if (isAdded)
				{
					break;
				}
			}

			if (!isAdded)
			{
				TopTools_MapOfShape occtEdges;
				m_graphDictionary.insert(std::make_pair(kpVertex->GetOcctVertex(), occtEdges));
			}
		}

		// 2. Add the vertices
		for (const Edge::Ptr& kpEdge: rkEdges)
		{
			const TopoDS_Edge& rkOcctEdge = kpEdge->GetOcctEdge();

			// a. Geometric check of the edge starts
			std::list<Vertex::Ptr> vertices;
			kpEdge->Vertices(vertices);
			for(const Vertex::Ptr& kpVertex : vertices)
			{
				TopoDS_Vertex occtVertex = kpVertex->GetOcctVertex();

				bool isVertexFound = false;
				TopoDS_Vertex occtFoundVertex;
				for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& kpPair : m_graphDictionary)
				{
					const TopoDS_Vertex& rkOcctExistingVertex = kpPair.first;
					BRepExtrema_DistShapeShape occtDistanceCalculation(occtVertex, rkOcctExistingVertex);
					bool isDone = occtDistanceCalculation.Perform();
					if (isDone)
					{
						double distance = occtDistanceCalculation.Value();
						if (distance < 0.0001)
						{
							isVertexFound = true;
							occtFoundVertex = rkOcctExistingVertex;
							break;
						}
					}
				}

				if (isVertexFound)
				{
					m_graphDictionary[occtFoundVertex].Add(rkOcctEdge);
				}
				else
				{
					TopTools_MapOfShape occtEdges;
					m_graphDictionary.insert(std::make_pair(occtVertex, occtEdges));
					Vertex::Ptr vertex = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtVertex));
				}
			} // for (const Vertex::Ptr& kpVertex : vertices)
		}
	}

	Graph::Graph(const Graph* kpAnotherGraph)
		: m_vertices(kpAnotherGraph->m_vertices)
		, m_edges(kpAnotherGraph->m_edges)
		, m_graphDictionary(kpAnotherGraph->m_graphDictionary)
	{
	}

	Graph::~Graph()
	{
		m_graphDictionary.clear();
		m_edges.clear();
		m_vertices.clear();
	}

	Topology::Ptr Graph::Topology() const
	{
		// Graph: visualise in this order:
		// 1. the edges
		// 2. isolated vertices

		// For a loop: circle, radius/diameter/circumference = average of the edge lengths

		std::list<Topology::Ptr> topologies;
		for (const Vertex::Ptr& kpVertex : m_vertices)
		{
			topologies.push_back(kpVertex);
		}
		for (const Edge::Ptr& kpEdge : m_edges)
		{
			topologies.push_back(kpEdge);
		}

		Cluster::Ptr cluster = Cluster::ByTopologies(topologies);
		return cluster;
	}

	void Graph::Vertices(std::list<std::shared_ptr<Vertex>>& vertices) const
	{
		vertices = m_vertices;
	}

	void Graph::Edges(std::list<std::shared_ptr<Edge>>& edges) const
	{
		edges = m_edges;
	}

	void Graph::AddVertex(const std::shared_ptr<Vertex>& kpVertex, const bool kAddToVerticesList)
	{
		//Graph::Ptr pAnotherGraph = std::make_shared<Graph>(this);
		bool isAdded = false;
		for (const Vertex::Ptr& kpExistingVertex : m_vertices)
		{
			isAdded = AreVerticesGeometricallyIdentical(kpVertex->GetOcctVertex(), kpExistingVertex->GetOcctVertex(), 0.00001);
			if (isAdded)
			{
				break;
			}
		}

		if(!isAdded)
		{
			if (kAddToVerticesList)
			{
				m_vertices.push_back(kpVertex);
			}
			m_graphDictionary.insert(std::make_pair(kpVertex->GetOcctVertex(), TopTools_MapOfShape()));
		}
	}

	void Graph::AddEdge(const std::shared_ptr<Edge>& kpEdge)
	{
		Vertex::Ptr pStartVertex = kpEdge->StartVertex();
		Vertex::Ptr pEndVertex = kpEdge->EndVertex();

		Vertex::Ptr pClosestToStartVertex = GetGeometricallyIdenticalVertexOrAddVertex(pStartVertex, true, false);
		Vertex::Ptr pClosestToEndVertex = GetGeometricallyIdenticalVertexOrAddVertex(pEndVertex, true, false);

		m_graphDictionary[pClosestToStartVertex->GetOcctVertex()].Add(pClosestToEndVertex->GetOcctVertex());
		m_graphDictionary[pClosestToEndVertex->GetOcctVertex()].Add(pClosestToStartVertex->GetOcctVertex());
		m_edges.push_back(kpEdge);
	}

	int Graph::VertexDegree(const std::shared_ptr<Vertex>& kpVertex) const
	{
		return VertexDegree(kpVertex->GetOcctVertex());
	}

	int Graph::VertexDegree(const TopoDS_Vertex & rkOcctVertex) const
	{
		TopoDS_Vertex occtClosestVertex = GetGeometricallyIdenticalVertex(rkOcctVertex);
		if (occtClosestVertex.IsNull())
		{
			return 0;
		}

		int numberOfEdges = m_graphDictionary.find(occtClosestVertex)->second.Size();
		int numberOfLoops = m_graphDictionary.find(occtClosestVertex)->second.Contains(occtClosestVertex) ? 1 : 0;
		int degree = numberOfEdges + numberOfLoops;

		return degree;
	}

	void Graph::AdjacentVertices(const std::shared_ptr<Vertex>& kpVertex, std::list<std::shared_ptr<Vertex>>& rAdjacentVertices) const
	{
		TopoDS_Vertex occtQueryVertex = kpVertex->GetOcctVertex();
		TopoDS_Vertex occtClosestVertex = GetGeometricallyIdenticalVertex(occtQueryVertex);
		if (occtClosestVertex.IsNull())
		{
			return;
		}

		TopTools_MapOfShape occtAdjacentVertices = m_graphDictionary.find(occtClosestVertex)->second;

		for (const Vertex::Ptr& kpExistingVertex : m_vertices)
		{
			TopoDS_Vertex occtExistingVertex = kpExistingVertex->GetOcctVertex();
			TopoDS_Vertex occtClosestVertexToExistingVertex = GetGeometricallyIdenticalVertex(occtExistingVertex);
			if (occtClosestVertexToExistingVertex.IsNull())
			{
				continue;
			}

			if (occtAdjacentVertices.Contains(occtClosestVertexToExistingVertex))
			{
				rAdjacentVertices.push_back(kpExistingVertex);
			}
		}
	}

	void Graph::Connect(const std::shared_ptr<Vertex>& kpVertex1, const std::shared_ptr<Vertex>& kpVertex2)
	{
		TopoDS_Vertex occtQueryVertex1 = kpVertex1->GetOcctVertex();
		TopoDS_Vertex occtClosestVertex1 = GetGeometricallyIdenticalVertex(occtQueryVertex1);
		if (occtClosestVertex1.IsNull())
		{
			occtClosestVertex1 = occtQueryVertex1;
		}

		TopoDS_Vertex occtQueryVertex2 = kpVertex2->GetOcctVertex();
		TopoDS_Vertex occtClosestVertex2 = GetGeometricallyIdenticalVertex(occtQueryVertex2);
		if (occtClosestVertex2.IsNull())
		{
			occtClosestVertex2 = occtQueryVertex2;
		}

		Edge::Ptr edge = Edge::ByStartVertexEndVertex(kpVertex1, kpVertex2);
		m_graphDictionary[occtClosestVertex1].Add(occtClosestVertex2);
		m_graphDictionary[occtClosestVertex2].Add(occtClosestVertex1);
		m_edges.push_back(edge);
	}

	bool Graph::ContainsVertex(const std::shared_ptr<Vertex>& kpVertex) const
	{
		return std::find(m_vertices.begin(), m_vertices.end(), kpVertex) != m_vertices.end();
	}

	bool Graph::ContainsEdge(const std::shared_ptr<Edge>& kpEdge)
	{
		return std::find(m_edges.begin(), m_edges.end(), kpEdge) != m_edges.end();
	}

	void Graph::DegreeSequence(std::list<int>& rDegreeSequence) const
	{
		 for (const Vertex::Ptr& kpVertex : m_vertices)
		 {
			 TopoDS_Vertex occtExistingVertex = kpVertex->GetOcctVertex();
			 TopoDS_Vertex occtClosestVertexToExistingVertex = GetGeometricallyIdenticalVertex(occtExistingVertex);
			 if (occtClosestVertexToExistingVertex.IsNull())
			 {
				 continue;
			 }

			 rDegreeSequence.push_back(VertexDegree(occtClosestVertexToExistingVertex));
		 }

		 rDegreeSequence.sort(std::greater<int>());
	}

	double Graph::Density() const
	{
		int numOfVertices = (int) m_vertices.size();
		int numOfEdges = (int) m_edges.size();
		double denominator = numOfVertices * (numOfVertices - 1);
		if (denominator > -0.0001 && denominator < 0.0001)
		{
			// Divide by zero, return the largest double number
			return std::numeric_limits<double>::max();
		}
		return (2 * numOfEdges) / denominator;
	}

	Graph::Ptr Graph::ByVertex(const Vertex::Ptr kpVertex,
		const bool kToExteriorApertures)
	{
		std::list<Vertex::Ptr> apertureCentresOfMass;
		if (kToExteriorApertures)
		{
			std::list<Topology::Ptr> contents;
			kpVertex->Contents(contents);

			for (const Topology::Ptr& kpContent : contents)
			{
				if (kpContent->GetType() == TOPOLOGY_APERTURE)
				{
					apertureCentresOfMass.push_back(kpContent->CenterOfMass());
				}
			}
		}

		std::list<Vertex::Ptr> vertices;
		std::list<Edge::Ptr> edges;
		for(const Vertex::Ptr& kpApertureCentreOfMass : apertureCentresOfMass)
		{ 
			Edge::Ptr edge = Edge::ByStartVertexEndVertex(kpVertex, kpApertureCentreOfMass);
			edges.push_back(edge);
		}
		if (edges.empty())
		{
			vertices.push_back(kpVertex);
		}
		return std::make_shared<Graph>(vertices, edges);
	}

	Graph::Ptr Graph::ByEdge(const Edge::Ptr kpEdge,
		const bool kDirect,
		const bool kToExteriorApertures)
	{
		std::list<Vertex::Ptr> vertices;
		std::list<Edge::Ptr> edges;
		std::list<Vertex::Ptr> edgeVertices;

		if (kDirect)
		{
			kpEdge->Vertices(vertices);
			edges.push_back(kpEdge);
		}

		std::list<Vertex::Ptr> apertureCentresOfMass;
		if (kToExteriorApertures)
		{
			std::list<Topology::Ptr> contents;
			kpEdge->Contents(contents);

			for (const Topology::Ptr& kpContent : contents)
			{
				if (kpContent->GetType() == TOPOLOGY_APERTURE)
				{
					Vertex::Ptr contentCenterOfMass = kpContent->CenterOfMass();
					vertices.push_back(contentCenterOfMass);
					std::list<Vertex::Ptr> edgeVertices;
					kpEdge->Vertices(edgeVertices);

					for (const Vertex::Ptr& kpVertex : edgeVertices)
					{
						Edge::Ptr edge = Edge::ByStartVertexEndVertex(kpVertex, contentCenterOfMass);
						edges.push_back(edge);
					}
				}
			}
		}

		return std::make_shared<Graph>(vertices, edges);
	}

	Graph::Ptr Graph::ByWire(const Wire::Ptr kpWire,
		const bool kDirect,
		const bool kToExteriorApertures)
	{
		std::list<Vertex::Ptr> vertices;
		std::list<Edge::Ptr> edges;
		if (kDirect || kToExteriorApertures)
		{
			kpWire->Edges(edges);
			kpWire->Vertices(vertices);
		}

		if (kToExteriorApertures)
		{
			// Iterate through the edges
			for (const Edge::Ptr& kpEdge : edges)
			{
				// Get the apertures
				std::list<Topology::Ptr> contents;
				kpEdge->Contents(contents);

				std::list<Vertex::Ptr> edgeVertices;
				kpEdge->Vertices(edgeVertices);

				for (const Topology::Ptr& kpContent : contents)
				{
					if (kpContent->GetType() == TOPOLOGY_APERTURE)
					{
						Vertex::Ptr contentCenterOfMass = kpContent->CenterOfMass();
						vertices.push_back(contentCenterOfMass);
						for (const Vertex::Ptr& edgeVertex :edgeVertices)
						{
							Edge::Ptr edge = Edge::ByStartVertexEndVertex(edgeVertex, contentCenterOfMass);
							edges.push_back(edge);
						}
					}
				}
			}
		}

		return std::make_shared<Graph>(vertices, edges);
	}

	Graph::Ptr Graph::ByFace(const Face::Ptr kpFace,
		const bool kToExteriorTopologies,
		const bool kToExteriorApertures)
	{
		std::list<Vertex::Ptr> vertices;
		std::list<Edge::Ptr> edges;

		Vertex::Ptr centerOfMass = kpFace->CenterOfMass();
		vertices.push_back(centerOfMass);
		if (kToExteriorTopologies || kToExteriorApertures)
		{
			std::list<Edge::Ptr> faceEdges;
			kpFace->Edges(faceEdges);

			for (const Edge::Ptr& kpFaceEdge : faceEdges)
			{
				if (kToExteriorTopologies)
				{
					Edge::Ptr edge = Edge::ByStartVertexEndVertex(centerOfMass, centerOfMass);
					edges.push_back(edge);
				}

				if (kToExteriorApertures)
				{
					std::list<Topology::Ptr> contents;
					kpFaceEdge->Contents(contents);

					for (const Topology::Ptr& kpContent : contents)
					{
						if (kpContent->GetType() == TOPOLOGY_APERTURE)
						{
							Vertex::Ptr pApertureCenterOfMass = kpContent->CenterOfMass();
							vertices.push_back(pApertureCenterOfMass);
							Edge::Ptr edge = Edge::ByStartVertexEndVertex(pApertureCenterOfMass, centerOfMass);
							edges.push_back(edge);
						}
					}
				}
			}
		}

		return std::make_shared<Graph>(vertices, edges);
	}

	Graph::Ptr Graph::ByShell(const Shell::Ptr kpShell,
		const bool kDirect,
		const bool kViaSharedTopologies,
		const bool kViaSharedApertures,
		const bool kToExteriorTopologies,
		const bool kToExteriorApertures)
	{
		if (kpShell == nullptr)
		{
			return nullptr;
		}

		// 1. Get the vertices mapped to their original topologies
		//    - Face --> centroid
		//   Occt shapes must be used as the keys. Topologic shapes cannot be used because there can be many shapes representing the same OCCT shapes.
		std::map<TopoDS_Face, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> faceCentroids;

		std::list<TopologicCore::Face::Ptr> faces;
		kpShell->Faces(faces);
		for (const TopologicCore::Face::Ptr& kpFace : faces)
		{
			TopologicCore::Vertex::Ptr pCentroid = kpFace->CenterOfMass();
			faceCentroids.insert(std::make_pair(kpFace->GetOcctFace(), pCentroid));
		}

		// 2. Check the configurations. Add the edges to a cluster.
		std::list<TopologicCore::Topology::Ptr> graphEdges;
		if (kDirect)
		{
			// Iterate through all faces and check for adjacency.
			// For each adjacent faces, connect the centroids
			for (const TopologicCore::Face::Ptr& kpFace : faces)
			{
				std::list<TopologicCore::Face::Ptr> adjacentFaces;
				kpFace->AdjacentFaces(adjacentFaces);

				for (const TopologicCore::Face::Ptr& kpAdjacentFace : adjacentFaces)
				{
					std::map<TopoDS_Face, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator>::iterator adjacentCentroidPair =
						faceCentroids.find(kpAdjacentFace->GetOcctFace());
					if (adjacentCentroidPair == faceCentroids.end())
					{
						continue;
					}
					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(faceCentroids[kpFace->GetOcctFace()], adjacentCentroidPair->second);
					graphEdges.push_back(pEdge);
				}
			}
		}

		std::list<TopologicCore::Edge::Ptr> edges;
		kpShell->Edges(edges);
		for (const TopologicCore::Edge::Ptr& kpEdge : edges)
		{
			TopologicCore::Vertex::Ptr pCentroid = kpEdge->CenterOfMass();
			bool isManifold = kpEdge->IsManifold();
			std::list<TopologicCore::Face::Ptr> adjacentFaces;
			kpEdge->Faces(adjacentFaces);

			std::list<Topology::Ptr> contents;
			kpEdge->Contents(contents);

			// Get the apertures and calculate their centroids
			//std::map<TopoDS_Shape, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> apertureCentroids;
			std::list<TopologicCore::Vertex::Ptr> apertureCentroids;
			for (const Topology::Ptr& kpContent : contents)
			{
				// If this is not an aperture, skip it
				if (kpContent->GetType() != TopologicCore::TOPOLOGY_APERTURE)
				{
					continue;
				}
				TopologicCore::Aperture::Ptr pAperture = TopologicalQuery::Downcast<TopologicCore::Aperture>(kpContent);
				Topology::Ptr pApertureTopology = pAperture->Topology();
				TopologicCore::Vertex::Ptr pApertureCentroid = pApertureTopology->CenterOfMass();

				apertureCentroids.push_back(pApertureCentroid);
			}

			// Check 
			for (const TopologicCore::Face::Ptr& kpAdjacentFace : adjacentFaces)
			{
				if ((!isManifold && kViaSharedTopologies) // i.e. non-manifold faces
					||
					(isManifold && kToExteriorTopologies))
				{
					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(pCentroid, faceCentroids[kpAdjacentFace->GetOcctFace()]);
					graphEdges.push_back(pEdge);
				}

				for (const TopologicCore::Vertex::Ptr& rkApertureCentroid : apertureCentroids)
				{
					if ((!isManifold && kViaSharedApertures) // i.e. non-manifold apertures
						||
						(isManifold && kToExteriorApertures))
					{
						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(rkApertureCentroid, faceCentroids[kpAdjacentFace->GetOcctFace()]);
						graphEdges.push_back(pEdge);
					}
				}
			}
		}

		// Merge the edges
		TopologicCore::Cluster::Ptr pTopologiesAsCluster = TopologicCore::Cluster::ByTopologies(graphEdges);
		if (pTopologiesAsCluster == nullptr)
		{
			return nullptr;
		}
		TopologicCore::Topology::Ptr pSelfMergedTopologies = pTopologiesAsCluster->SelfMerge();

		// If this is a cluster, use it to create a dual graph. Otherwise, add it to a cluster to be used to create a dual graph.
		TopologicCore::Cluster::Ptr pSelfMergedTopologiesAsCluster = std::dynamic_pointer_cast<Cluster>(pSelfMergedTopologies);
		std::list<Vertex::Ptr> vertices;
		if (pSelfMergedTopologiesAsCluster != nullptr)
		{
			std::list<Edge::Ptr> mergedGraphEdges;
			pSelfMergedTopologies->Vertices(vertices);
			pSelfMergedTopologies->Edges(mergedGraphEdges);
			return std::make_shared<Graph>(vertices, mergedGraphEdges);
		}

		// else 
		std::list<Edge::Ptr> finalGraphEdges; // converted to Edge::Ptr
		for (const Topology::Ptr& kpEdgeTopology : graphEdges)
		{
			std::list<Vertex::Ptr> edgeVertices;
			kpEdgeTopology->Vertices(edgeVertices);
			for (const Vertex::Ptr& kpVertex : edgeVertices)
			{
				vertices.push_back(kpVertex);
			}
			Edge::Ptr edge = std::dynamic_pointer_cast<Edge>(kpEdgeTopology);
			finalGraphEdges.push_back(edge);
		}
		return std::make_shared<Graph>(vertices, finalGraphEdges);
	}

	Graph::Ptr Graph::ByCell(const Cell::Ptr kpCell,
		const bool kToExteriorTopologies,
		const bool kToExteriorApertures)
	{
		std::list<Vertex::Ptr> vertices;
		std::list<Edge::Ptr> edges;

		Vertex::Ptr centerOfMass = kpCell->CenterOfMass();
		vertices.push_back(kpCell->CenterOfMass());
		if (kToExteriorTopologies || kToExteriorApertures)
		{
			std::list<Face::Ptr> cellFaces;
			kpCell->Faces(cellFaces);

			for (const Face::Ptr& kpCellFace : cellFaces)
			{
				if (kToExteriorTopologies)
				{
					Vertex::Ptr pCellFaceCenterOfMass = kpCellFace->CenterOfMass();
					vertices.push_back(pCellFaceCenterOfMass);
					Edge::Ptr edge = Edge::ByStartVertexEndVertex(pCellFaceCenterOfMass, centerOfMass);
					edges.push_back(edge);
				}

				if (kToExteriorApertures)
				{
					std::list<Topology::Ptr> contents;
					kpCellFace->Contents(contents);

					for (const Topology::Ptr& kpContent : contents)
					{
						if (kpContent->GetType() == TOPOLOGY_APERTURE)
						{
							Vertex::Ptr pApertureCenterOfMass = kpContent->CenterOfMass();
							vertices.push_back(pApertureCenterOfMass);
							Edge::Ptr edge = Edge::ByStartVertexEndVertex(pApertureCenterOfMass, centerOfMass);
							edges.push_back(edge);
						}
					}
				}
			}
		}

		return std::make_shared<Graph>(vertices, edges);
	}

	Graph::Ptr Graph::ByCellComplex(const CellComplex::Ptr kpCellComplex,
		const bool kDirect,
		const bool kViaSharedTopologies,
		const bool kViaSharedApertures,
		const bool kToExteriorTopologies,
		const bool kToExteriorApertures)
	{
		if (kpCellComplex == nullptr)
		{
			return nullptr;
		}

		// 1. Get the vertices mapped to their original topologies
		//    - Cell --> centroid
		//   Occt shapes must be used as the keys. Topologic shapes cannot be used because there can be many shapes representing the same OCCT shapes.
		std::map<TopoDS_Solid, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> cellCentroids;

		std::list<TopologicCore::Cell::Ptr> cells;
		kpCellComplex->Cells(cells);
		for (const TopologicCore::Cell::Ptr& kpCell : cells)
		{
			TopologicCore::Vertex::Ptr pCentroid = kpCell->CenterOfMass();
			cellCentroids.insert(std::make_pair(kpCell->GetOcctSolid(), pCentroid));
		}

		// 2. Check the configurations. Add the edges to a cluster.
		std::list<TopologicCore::Topology::Ptr> edges;
		if (kDirect)
		{
			// Iterate through all cells and check for adjacency.
			// For each adjacent cells, connect the centroids
			for (const TopologicCore::Cell::Ptr& kpCell : cells)
			{
				std::list<TopologicCore::Cell::Ptr> adjacentCells;
				kpCell->AdjacentCells(adjacentCells);

				for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
				{
					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(cellCentroids[kpCell->GetOcctSolid()], cellCentroids[kpAdjacentCell->GetOcctSolid()]);
					edges.push_back(pEdge);
				}
			}
		}

		std::list<TopologicCore::Face::Ptr> faces;
		kpCellComplex->Faces(faces);
		for (const TopologicCore::Face::Ptr& kpFace : faces)
		{
			TopologicCore::Vertex::Ptr pCentroid = kpFace->CenterOfMass();
			bool isManifold = kpFace->IsManifold();
			std::list<TopologicCore::Cell::Ptr> adjacentCells;
			kpFace->Cells(adjacentCells);

			std::list<Topology::Ptr> contents;
			kpFace->Contents(contents);

			// Get the apertures and calculate their centroids
			//std::map<TopoDS_Shape, TopologicCore::Vertex::Ptr, TopologicCore::OcctShapeComparator> apertureCentroids;
			std::list<TopologicCore::Vertex::Ptr> apertureCentroids;
			for (const Topology::Ptr& kpContent : contents)
			{
				// If this is not an aperture, skip it
				if (kpContent->GetType() != TopologicCore::TOPOLOGY_APERTURE)
				{
					continue;
				}
				TopologicCore::Aperture::Ptr pAperture = TopologicalQuery::Downcast<TopologicCore::Aperture>(kpContent);
				Topology::Ptr pApertureTopology = pAperture->Topology();
				TopologicCore::Vertex::Ptr pApertureCentroid = pApertureTopology->CenterOfMass();

				apertureCentroids.push_back(pApertureCentroid);
			}

			// Check 
			for (const TopologicCore::Cell::Ptr& kpAdjacentCell : adjacentCells)
			{
				if ((!isManifold && kViaSharedTopologies) // i.e. non-manifold faces
					||
					(isManifold && kToExteriorTopologies))
				{
					TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(pCentroid, cellCentroids[kpAdjacentCell->GetOcctSolid()]);
					edges.push_back(pEdge);
				}

				for (const TopologicCore::Vertex::Ptr& rkApertureCentroid : apertureCentroids)
				{
					if ((!isManifold && kViaSharedApertures) // i.e. non-manifold apertures
						||
						(isManifold && kToExteriorApertures))
					{
						TopologicCore::Edge::Ptr pEdge = TopologicCore::Edge::ByStartVertexEndVertex(rkApertureCentroid, cellCentroids[kpAdjacentCell->GetOcctSolid()]);
						edges.push_back(pEdge);
					}
				}
			}
		}

		// 3. Self-merge the cluster

		// Merge the edges
		TopologicCore::Cluster::Ptr pTopologiesAsCluster = TopologicCore::Cluster::ByTopologies(edges);
		if (pTopologiesAsCluster == nullptr)
		{
			return nullptr;
		}
		TopologicCore::Topology::Ptr pSelfMergedTopologies = pTopologiesAsCluster->SelfMerge();

		// If this is a cluster, use it to create a dual graph. Otherwise, add it to a cluster to be used to create a dual graph.
		TopologicCore::Cluster::Ptr pSelfMergedTopologiesAsCluster = std::dynamic_pointer_cast<Cluster>(pSelfMergedTopologies);
		std::list<Vertex::Ptr> vertices;
		if (pSelfMergedTopologiesAsCluster != nullptr)
		{
			std::list<Edge::Ptr> mergedGraphEdges;
			pSelfMergedTopologies->Vertices(vertices);
			pSelfMergedTopologies->Edges(mergedGraphEdges);
			return std::make_shared<Graph>(vertices, mergedGraphEdges);
		}

		// else 
		std::list<Edge::Ptr> finalGraphEdges; // converted to Edge::Ptr
		for (const Topology::Ptr& kpEdgeTopology : edges)
		{
			std::list<Vertex::Ptr> edgeVertices;
			kpEdgeTopology->Vertices(edgeVertices);
			for (const Vertex::Ptr& kpVertex : edgeVertices)
			{
				vertices.push_back(kpVertex);
			}
			finalGraphEdges.push_back(std::dynamic_pointer_cast<Edge>(kpEdgeTopology));
		}
		return std::make_shared<Graph>(vertices, finalGraphEdges);
	}

	Graph::Ptr Graph::ByCluster(const Cluster::Ptr cluster,
		const bool kDirect,
		const bool kViaSharedTopologies,
		const bool kViaSharedApertures,
		const bool kToExteriorTopologies,
		const bool kToExteriorApertures)
	{
		std::list<Topology::Ptr> subtopologies;
		cluster->SubTopologies(subtopologies);
		std::list<Vertex::Ptr> vertices;
		std::list<Edge::Ptr> edges;
		for (const Topology::Ptr& kpSubtopology : subtopologies)
		{
			Graph::Ptr graph = Graph::ByTopology(kpSubtopology, kDirect, kViaSharedTopologies, kViaSharedApertures, kToExteriorTopologies, kToExteriorApertures);
			vertices.insert(vertices.end(), graph->m_vertices.begin(), graph->m_vertices.end());
			edges.insert(edges.end(), graph->m_edges.begin(), graph->m_edges.end());
		}
		return std::make_shared<Graph>(vertices, edges);
	}

	bool Graph::AreVerticesGeometricallyIdentical(const TopoDS_Vertex & rkOcctVertex1, const TopoDS_Vertex & rkOcctVertex2, const double kDistanceThreshold)
	{
		BRepExtrema_DistShapeShape occtDistanceCalculation(rkOcctVertex1, rkOcctVertex2);
		bool isDone = occtDistanceCalculation.Perform();
		if (isDone)
		{
			double distance = occtDistanceCalculation.Value();
			if (distance < kDistanceThreshold)
			{
				return true;
			}
		}
		return false;
	}

	std::shared_ptr<Vertex> Graph::GetGeometricallyIdenticalVertexOrAddVertex(const std::shared_ptr<Vertex>& kpVertex, const bool kAddToDictionary, const bool kAddToVerticesList)
	{
		for (const Vertex::Ptr& kpExistingVertex : m_vertices)
		{
			bool isFound = AreVerticesGeometricallyIdentical(kpVertex->GetOcctVertex(), kpExistingVertex->GetOcctVertex(), 0.0001);
			if (isFound)
			{
				return kpExistingVertex;
			}
		}
		
		if (kAddToDictionary)
		{
			AddVertex(kpVertex, kAddToVerticesList);
			return kpVertex;
		}
		//else
		return nullptr;
	}

	TopoDS_Vertex Graph::GetGeometricallyIdenticalVertex(const TopoDS_Vertex& rkOcctVertex) const
	{
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& vertexEdgesPair: m_graphDictionary)
		{
			TopoDS_Vertex occtExistingVertex = vertexEdgesPair.first;

			bool isFound = AreVerticesGeometricallyIdentical(rkOcctVertex, occtExistingVertex, 0.0001);
			if (isFound)
			{
				return occtExistingVertex;
			}
		}

		return TopoDS_Vertex();
	}

	std::shared_ptr<Vertex> Graph::GetGeometricallyIdenticalVertex(const std::shared_ptr<Vertex>& kpQueryVertex) const
	{
		TopoDS_Vertex occtQueryVertex = kpQueryVertex->GetOcctVertex();
		TopoDS_Vertex occtClosestVertex = GetGeometricallyIdenticalVertex(occtQueryVertex);
		if (occtClosestVertex.IsNull())
		{
			return nullptr;
		}

		return std::make_shared<Vertex>(occtClosestVertex);
	}
}