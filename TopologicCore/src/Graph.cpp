#include "Graph.h"
#include "Cluster.h"
#include "Wire.h"
#include "Face.h"
#include "Shell.h"
#include "Cell.h"
#include "CellComplex.h"
#include "Aperture.h"

#include <BRepExtrema_DistShapeShape.hxx>
#include <TopoDS.hxx>

#include <algorithm>
#include <numeric>

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
	{
		// 1. Add the vertices
		AddVertices(rkVertices, false, 0.0);

		// 2. Add the edges
		AddEdges(rkEdges, false, 0.0);
	}

	Graph::Graph(const Graph* kpAnotherGraph)
		: m_graphDictionary(kpAnotherGraph->m_graphDictionary)
	{
	}

	Graph::~Graph()
	{
		m_graphDictionary.clear();
	}

	Topology::Ptr Graph::Topology() const
	{
		// Graph: visualise in this order:
		// 1. the edges
		// 2. isolated vertices

		// For a loop: circle, radius/diameter/circumference = average of the edge lengths

		std::list<Topology::Ptr> topologies;
		GraphMap processedAdjacency;
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& rkDictionaryPair : m_graphDictionary)
		{
			TopoDS_Vertex occtVertex1 = rkDictionaryPair.first;
			Vertex::Ptr vertex1 = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtVertex1));
			if (rkDictionaryPair.second.Size() == 0)
			{
				// Just add the vertex
				topologies.push_back(vertex1);
				processedAdjacency.insert(std::make_pair(occtVertex1, TopTools_MapOfShape()));
			}
			else
			{
				// Create edges
				for (TopTools_MapIteratorOfMapOfShape mapIterator(rkDictionaryPair.second);
					mapIterator.More();
					mapIterator.Next())
				{
					TopoDS_Shape occtVertex2 = mapIterator.Value();
					Vertex::Ptr vertex2 = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtVertex2));

					GraphMap::iterator vertex1Iterator = processedAdjacency.find(occtVertex1);
					if (vertex1Iterator != processedAdjacency.end())
					{
						if (vertex1Iterator->second.Contains(occtVertex2))
						{
							continue;
						}
					}

					GraphMap::iterator vertex2Iterator = processedAdjacency.find(TopoDS::Vertex(occtVertex2));
					if (vertex2Iterator != processedAdjacency.end())
					{
						if (vertex2Iterator->second.Contains(occtVertex1))
						{
							continue;
						}
					}

					Edge::Ptr edge = Edge::ByStartVertexEndVertex(vertex1, vertex2);
					topologies.push_back(edge);
					processedAdjacency[vertex1->GetOcctVertex()].Add(vertex2->GetOcctVertex());
					processedAdjacency[vertex2->GetOcctVertex()].Add(vertex1->GetOcctVertex());
				}
			}
		}

		Cluster::Ptr cluster = Cluster::ByTopologies(topologies);
		return cluster;
	}

	void Graph::Vertices(std::list<std::shared_ptr<Vertex>>& vertices) const
	{
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& rkDictionaryPair : m_graphDictionary)
		{
			Vertex::Ptr vertex = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(rkDictionaryPair.first));
			vertices.push_back(vertex);
		}
	}

	void Graph::Edges(std::list<std::shared_ptr<Edge>>& edges) const
	{
		GraphMap processedAdjacency;
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& rkDictionaryPair : m_graphDictionary)
		{
			TopoDS_Vertex occtVertex1 = rkDictionaryPair.first;
			Vertex::Ptr vertex1 = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtVertex1));
			// Create edges
			for (TopTools_MapIteratorOfMapOfShape mapIterator(rkDictionaryPair.second);
				mapIterator.More();
				mapIterator.Next())
			{
				TopoDS_Shape occtVertex2 = mapIterator.Value();
				GraphMap::iterator vertex1Iterator = processedAdjacency.find(occtVertex1);
				if (vertex1Iterator != processedAdjacency.end())
				{
					if (vertex1Iterator->second.Contains(occtVertex2))
					{
						continue;
					}
				}

				GraphMap::iterator vertex2Iterator = processedAdjacency.find(TopoDS::Vertex(occtVertex2));
				if (vertex2Iterator != processedAdjacency.end())
				{
					if (vertex2Iterator->second.Contains(occtVertex1))
					{
						continue;
					}
				}

				Topology::Ptr topology = Topology::ByOcctShape(TopoDS::Vertex(occtVertex2));
				Vertex::Ptr vertex2 = std::dynamic_pointer_cast<Vertex>(topology);
				Edge::Ptr edge = Edge::ByStartVertexEndVertex(vertex1, vertex2);
				edges.push_back(edge);
				processedAdjacency[vertex1->GetOcctVertex()].Add(vertex2->GetOcctVertex());
				processedAdjacency[vertex2->GetOcctVertex()].Add(vertex1->GetOcctVertex());
			}
		}
	}

	void Graph::AddVertices(const std::list<Vertex::Ptr>& rkVertices, const bool kUseTolerance, const double kTolerance)
	{
		for (const Vertex::Ptr& kpVertex : rkVertices)
		{
			if (!ContainsVertex(kpVertex, kUseTolerance, kTolerance))
			{
				m_graphDictionary.insert(std::make_pair(kpVertex->GetOcctVertex(), TopTools_MapOfShape()));
			}
		}
	}

	void Graph::AddEdges(const std::list<Edge::Ptr>& rkEdges, const bool kUseTolerance, const double kTolerance)
	{
		for (const Edge::Ptr& kpEdge : rkEdges)
		{
			if (!ContainsEdge(kpEdge, kUseTolerance, kTolerance))
			{
				Vertex::Ptr startVertex = kpEdge->StartVertex();
				TopoDS_Vertex occtStartCoincidentVertex = GetCoincidentVertex(startVertex->GetOcctVertex(), kUseTolerance, kTolerance);
				if (occtStartCoincidentVertex.IsNull())
				{
					occtStartCoincidentVertex = startVertex->GetOcctVertex();
				}
				Vertex::Ptr endVertex = kpEdge->EndVertex();
				TopoDS_Vertex occtEndCoincidentVertex = GetCoincidentVertex(endVertex->GetOcctVertex(), kUseTolerance, kTolerance);
				if (occtEndCoincidentVertex.IsNull())
				{
					occtEndCoincidentVertex = endVertex->GetOcctVertex();
				}

				m_graphDictionary[occtStartCoincidentVertex].Add(occtEndCoincidentVertex);
				m_graphDictionary[occtEndCoincidentVertex].Add(occtStartCoincidentVertex);
			}
		}
	}

	int Graph::Degree(const std::shared_ptr<Vertex>& kpVertex) const
	{
		return Degree(kpVertex->GetOcctVertex());
	}

	int Graph::Degree(const TopoDS_Vertex & rkOcctVertex) const
	{
		if (m_graphDictionary.find(rkOcctVertex) == m_graphDictionary.end())
		{
			return 0;
		}

		int numberOfEdges = m_graphDictionary.find(rkOcctVertex)->second.Size();
		int numberOfLoops = m_graphDictionary.find(rkOcctVertex)->second.Contains(rkOcctVertex) ? 1 : 0;
		int degree = numberOfEdges + numberOfLoops;

		return degree;
	}

	void Graph::AdjacentVertices(const std::shared_ptr<Vertex>& kpVertex, std::list<std::shared_ptr<Vertex>>& rAdjacentVertices) const
	{
		TopoDS_Vertex occtQueryVertex = kpVertex->GetOcctVertex();
		if (!ContainsVertex(occtQueryVertex, false, 0.0))
		{
			return;
		}

		TopTools_MapOfShape occtAdjacentVertices = m_graphDictionary.find(occtQueryVertex)->second;
		for (TopTools_MapIteratorOfMapOfShape mapIterator(occtAdjacentVertices);
			mapIterator.More();
			mapIterator.Next())
		{
			Vertex::Ptr adjacentVertex = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(mapIterator.Value()));
			rAdjacentVertices.push_back(adjacentVertex);
		}
	}

	void Graph::Connect(const std::shared_ptr<Vertex>& kpVertex1, const std::shared_ptr<Vertex>& kpVertex2, const bool kUseTolerance, const double kTolerance)
	{
		TopoDS_Vertex occtQueryVertex1 = GetCoincidentVertex(kpVertex1->GetOcctVertex(), kUseTolerance, kTolerance);
		if (occtQueryVertex1.IsNull())
		{
			occtQueryVertex1 = kpVertex1->GetOcctVertex();
		}
		TopoDS_Vertex occtQueryVertex2 = GetCoincidentVertex(kpVertex2->GetOcctVertex(), kUseTolerance, kTolerance);
		if (occtQueryVertex2.IsNull())
		{
			occtQueryVertex2 = kpVertex2->GetOcctVertex();
		}

		m_graphDictionary[occtQueryVertex1].Add(occtQueryVertex2);
		m_graphDictionary[occtQueryVertex2].Add(occtQueryVertex1);
	}

	bool Graph::ContainsVertex(const std::shared_ptr<Vertex>& kpVertex, const bool kUseTolerance, const double kTolerance) const
	{
		return ContainsVertex(kpVertex->GetOcctVertex(), kUseTolerance, kTolerance);
	}

	bool Graph::ContainsVertex(const TopoDS_Vertex & rkOcctVertex, const bool kUseTolerance, const double kTolerance) const
	{
		TopoDS_Vertex occtCoincidentVertex = GetCoincidentVertex(rkOcctVertex, kUseTolerance, kTolerance);
		return !occtCoincidentVertex.IsNull();
	}

	bool Graph::ContainsEdge(const std::shared_ptr<Edge>& kpEdge, const bool kUseTolerance, const double kTolerance)
	{
		Vertex::Ptr startVertex = kpEdge->StartVertex();
		TopoDS_Vertex occtStartCoincidentVertex = GetCoincidentVertex(startVertex->GetOcctVertex(), kUseTolerance, kTolerance);
		if (occtStartCoincidentVertex.IsNull())
		{
			return false;
		}
		Vertex::Ptr endVertex = kpEdge->EndVertex();
		TopoDS_Vertex occtEndCoincidentVertex = GetCoincidentVertex(endVertex->GetOcctVertex(), kUseTolerance, kTolerance);
		if (occtEndCoincidentVertex.IsNull())
		{
			return false;
		}

		TopTools_MapOfShape adjacentVerticesToStart = m_graphDictionary[occtStartCoincidentVertex];
		TopTools_MapOfShape adjacentVerticesToEnd = m_graphDictionary[occtEndCoincidentVertex];

		return adjacentVerticesToStart.Contains(occtEndCoincidentVertex) || adjacentVerticesToEnd.Contains(occtStartCoincidentVertex);
	}

	void Graph::DegreeSequence(std::list<int>& rDegreeSequence) const
	{
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& rkDictionaryPair : m_graphDictionary)
		{
			Vertex::Ptr vertex = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(rkDictionaryPair.first));
			rDegreeSequence.push_back(Degree(vertex));
		}

		 rDegreeSequence.sort(std::greater<int>());
	}

	double Graph::Density() const
	{
		int numOfVertices = (int) m_graphDictionary.size();

		std::list<Edge::Ptr> edges;
		Edges(edges);
		int numOfEdges = (int)edges.size();
		double denominator = numOfVertices * (numOfVertices - 1);
		if (denominator > -0.0001 && denominator < 0.0001)
		{
			// Divide by zero, return the largest double number
			return std::numeric_limits<double>::max();
		}
		return  (2 * numOfEdges) / denominator;
		//return numOfEdges / denominator; 
	}

	bool Graph::IsComplete() const
	{
		return Density() > 0.9999;
	}

	void Graph::IsolatedVertices(std::list<Vertex::Ptr>& rIsolatedVertices) const
	{
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& kpPair : m_graphDictionary)
		{
			if (kpPair.second.IsEmpty())
			{
				rIsolatedVertices.push_back(std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(kpPair.first)));
			}
		}
	}

	int Graph::MinimumDelta() const
	{
		int minimumDelta = std::numeric_limits<int>::max();

		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& kpPair : m_graphDictionary)
		{
			int vertexDegree = Degree(kpPair.first);
			if (vertexDegree < minimumDelta)
			{
				minimumDelta = vertexDegree;
			}
		}
		return minimumDelta;
	}

	int Graph::MaximumDelta() const
	{
		int maximumDelta = 0;

		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& kpPair : m_graphDictionary)
		{
			int vertexDegree = Degree(kpPair.first);
			if (vertexDegree > maximumDelta)
			{
				maximumDelta = vertexDegree;
			}
		}
		return maximumDelta;
	}

	void Graph::AllPaths(
		const Vertex::Ptr & kpStartVertex, 
		const Vertex::Ptr & kpEndVertex,
		const bool kUseTimeLimit,
		const double kTimeLimitInSeconds,
		std::list<Wire::Ptr>& rPaths) const
	{
		std::list<Vertex::Ptr> path;
		auto currentTime = std::chrono::system_clock::now();
		AllPaths(kpStartVertex, kpEndVertex, kUseTimeLimit, kTimeLimitInSeconds, currentTime, path, rPaths);
	}

	void Graph::AllPaths(
		const Vertex::Ptr & kpStartVertex, 
		const Vertex::Ptr & kpEndVertex,
		const bool kUseTimeLimit,
		const double kTimeLimitInSeconds,
		const std::chrono::system_clock::time_point& rkStartingTime,
		std::list<Vertex::Ptr>& rPath, 
		std::list<Wire::Ptr>& rPaths) const
	{
		// Check elapsed time
		if (kUseTimeLimit)
		{
			auto currentTime = std::chrono::system_clock::now();
			auto timeDifferenceInSeconds = std::chrono::duration_cast<std::chrono::seconds>(currentTime - rkStartingTime);
			if (timeDifferenceInSeconds.count() >= kTimeLimitInSeconds)
			{
				return;
			}
		}

		if (!ContainsVertex(kpStartVertex, false, 0.0))
		{
			return;
		}

		rPath.push_back(kpStartVertex);
		if (kpStartVertex->IsSame(kpEndVertex))
		{
			// Create a wire
			Wire::Ptr pathWire = ConstructPath(rPath);
			rPaths.push_back(pathWire);
			return;
		}

		TopTools_MapOfShape occtConnectedVertices = m_graphDictionary.find(kpStartVertex->GetOcctVertex())->second;
		for (TopTools_MapIteratorOfMapOfShape mapIterator(occtConnectedVertices);
			mapIterator.More();
			mapIterator.Next())
		{
			Vertex::Ptr connectedVertex = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(mapIterator.Value()));
			if (std::find_if(rPath.begin(), rPath.end(), TopologyCompare(connectedVertex)) == rPath.end())
			{
				std::list<Wire::Ptr> extendedPaths;
				std::list<Vertex::Ptr> previousPath = rPath;
				AllPaths(connectedVertex, kpEndVertex, kUseTimeLimit, kTimeLimitInSeconds, rkStartingTime, previousPath, extendedPaths);
				for (const Wire::Ptr& rkExtendedPath : extendedPaths)
				{
					rPaths.push_back(rkExtendedPath);
				}
			}
		}
	}

	Wire::Ptr Graph::Path(const Vertex::Ptr & kpStartVertex, const Vertex::Ptr & kpEndVertex) const
	{
		std::list<Vertex::Ptr> path;
		return Path(kpStartVertex, kpEndVertex, path);
	}

	Wire::Ptr Graph::Path(const Vertex::Ptr & kpStartVertex, const Vertex::Ptr & kpEndVertex, std::list<Vertex::Ptr>& rPath) const
	{
		rPath.push_back(kpStartVertex);
		if (!ContainsVertex(kpStartVertex, false, 0.0))
		{
			return nullptr;
		}

		if (kpStartVertex->IsSame(kpEndVertex))
		//if (kpStartVertex == kpEndVertex)
		{
			Wire::Ptr pathWire = ConstructPath(rPath);
			return pathWire;
		}

		TopTools_MapOfShape occtConnectedVertices = m_graphDictionary.find(kpStartVertex->GetOcctVertex())->second;
		for (TopTools_MapIteratorOfMapOfShape mapIterator(occtConnectedVertices);
			mapIterator.More();
			mapIterator.Next())
		{
			Vertex::Ptr connectedVertex = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(mapIterator.Value()));
			if (std::find_if(rPath.begin(), rPath.end(), TopologyCompare(connectedVertex)) == rPath.end())
			{
				Wire::Ptr extendedPath = Path(connectedVertex, kpEndVertex, rPath);
				if (extendedPath != nullptr)
				{
					return extendedPath;
				}
			}
		}

		return nullptr;
	}

	std::shared_ptr<Wire> Graph::ShortestPath(const Vertex::Ptr & kpStartVertex, const Vertex::Ptr & kpEndVertex) const
	{
		return ShortestPath(kpStartVertex->GetOcctVertex(), kpEndVertex->GetOcctVertex());
	}

	std::shared_ptr<Wire> Graph::ShortestPath(const TopoDS_Vertex & rkOcctStartVertex, const TopoDS_Vertex & rkOcctEndVertex) const
	{
		// Dijkstra's: https://en.wikipedia.org/wiki/Dijkstra%27s_algorithm#Pseudocode
		std::list<TopoDS_Vertex> vertexList;
		std::map<TopoDS_Vertex, double, OcctShapeComparator> distanceMap;
		std::map<TopoDS_Vertex, TopoDS_Vertex, OcctShapeComparator> parentMap;
		for (const std::pair<TopoDS_Vertex, TopTools_MapOfShape>& vertexEdgesPair : m_graphDictionary)
		{
			distanceMap[vertexEdgesPair.first] = std::numeric_limits<double>::max();
			parentMap[vertexEdgesPair.first] = TopoDS_Vertex();
			vertexList.push_back(vertexEdgesPair.first);
		}

		distanceMap[rkOcctStartVertex] = 0;

		while (!vertexList.empty())
		{
			// Find vertex with the lowest distance
			double minDistance = std::numeric_limits<double>::max();
			TopoDS_Vertex occtVertexMinDistance;
			for (const TopoDS_Vertex& kpVertexInQueue : vertexList)
			{
				double distance = distanceMap[kpVertexInQueue];
				if (distance < minDistance)
				{
					minDistance = distance;
					occtVertexMinDistance = kpVertexInQueue;
				}
			}

			vertexList.remove(occtVertexMinDistance);

			if (occtVertexMinDistance.IsNull())
			{
				continue;
			}

			if (occtVertexMinDistance == rkOcctEndVertex)
			{
				std::list<Vertex::Ptr> path;
				//path.push_front(std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(rkOcctEndVertex)));
				TopoDS_Vertex occtCurrentVertex = occtVertexMinDistance;
				if (parentMap.find(occtCurrentVertex) != parentMap.end() ||
					occtVertexMinDistance == rkOcctStartVertex)
				{
					while (!occtCurrentVertex.IsNull())
					{
						path.push_front(std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtCurrentVertex)));
						std::map<TopoDS_Vertex, TopoDS_Vertex>::iterator parentIterator = parentMap.find(occtCurrentVertex);
						if (parentIterator != parentMap.end())
						{
							occtCurrentVertex = parentIterator->second;
						}
						else
						{
							occtCurrentVertex = TopoDS_Vertex();
						}
					}
				}

				return ConstructPath(path);
			}

			TopTools_MapOfShape occtVertexMinDistanceNeighbours = m_graphDictionary.find(occtVertexMinDistance)->second;
			for (TopTools_MapIteratorOfMapOfShape mapIterator(occtVertexMinDistanceNeighbours);
				mapIterator.More();
				mapIterator.Next())
			{
				TopoDS_Vertex occtNeighbour = TopoDS::Vertex(mapIterator.Value());
				double length = 0.0;
				if (!occtVertexMinDistance.IsSame(occtNeighbour))
				{
					length = 1.0; // to be replaced by the cost
				}
				double alternativeDistance = distanceMap[occtVertexMinDistance] + length;
				if (alternativeDistance < distanceMap[occtNeighbour])
				{
					distanceMap[occtNeighbour] = alternativeDistance;
					parentMap[occtNeighbour] = occtVertexMinDistance;
				}
			}
		}

		return nullptr;
	}

	int Graph::Diameter() const
	{
		std::map<Vertex::Ptr, Vertex::Ptr> vertexPairs;
		for (std::map<TopoDS_Vertex, TopTools_MapOfShape, OcctShapeComparator>::const_iterator kVertexEdgesPair1 = m_graphDictionary.begin();
			kVertexEdgesPair1 != m_graphDictionary.end();
			kVertexEdgesPair1++)
		{
			TopoDS_Vertex occtVertex1 = kVertexEdgesPair1->first;
			Vertex::Ptr vertex1 = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtVertex1));

			for (std::map<TopoDS_Vertex, TopTools_MapOfShape, OcctShapeComparator>::const_iterator kVertexEdgesPair2 = kVertexEdgesPair1;
				kVertexEdgesPair2 != m_graphDictionary.end();
				kVertexEdgesPair2++)
			{
				if (kVertexEdgesPair1 == kVertexEdgesPair2)
				{
					continue;
				}

				TopoDS_Vertex occtVertex2 = kVertexEdgesPair2->first;
				Vertex::Ptr vertex2 = std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtVertex2));

				vertexPairs.insert(std::pair<Vertex::Ptr, Vertex::Ptr>(vertex1, vertex2));
			}
		}

		int maxShortestPathDistance = 0;
		for (const std::pair<Vertex::Ptr, Vertex::Ptr>& rkVertexPair : vertexPairs)
		{
			int distance = Distance(rkVertexPair.first, rkVertexPair.second);
			if (distance > maxShortestPathDistance)
			{
				maxShortestPathDistance = distance;
			}
		}

		return maxShortestPathDistance;
	}

	int Graph::Distance(const std::shared_ptr<Vertex>& kpStartVertex, const std::shared_ptr<Vertex>& kpEndVertex) const
	{
		return Distance(kpStartVertex->GetOcctVertex(), kpEndVertex->GetOcctVertex());
	}

	int Graph::Distance(const TopoDS_Vertex & rkOcctStartVertex, const TopoDS_Vertex & rkOcctVertex) const
	{
		Wire::Ptr shortestPath = ShortestPath(rkOcctStartVertex, rkOcctVertex);
		std::list<Vertex::Ptr> vertices;
		shortestPath->Vertices(vertices);
		return (int)vertices.size() - 1;
	}

	int Graph::Eccentricity(const std::shared_ptr<Vertex>& kpVertex) const
	{
		std::map<TopoDS_Vertex, TopTools_MapOfShape, OcctShapeComparator>::const_iterator occtAdjacentVerticesPair =
			m_graphDictionary.find(kpVertex->GetOcctVertex());
		if (occtAdjacentVerticesPair == m_graphDictionary.end())
		{
			return std::numeric_limits<int>::max(); // infinite distance
		}

		TopTools_MapOfShape occtAdjacentVertices = occtAdjacentVerticesPair->second; 
		int eccentricity = 0;
		for (TopTools_MapIteratorOfMapOfShape mapIterator(occtAdjacentVertices);
			mapIterator.More();
			mapIterator.Next())
		{
			int distance = Distance(occtAdjacentVerticesPair->first, TopoDS::Vertex(mapIterator.Value()));
			if (distance > eccentricity)
			{
				eccentricity = distance;
			}
		}

		return eccentricity;
	}

	bool Graph::IsErdoesGallai(const std::list<int>& rkSequence) const
	{
		int sum = std::accumulate(rkSequence.begin(), rkSequence.end(), 0);
		if (sum % 2 != 0)
		{
			return false;
		}

		if (IsDegreeSequence(rkSequence))
		{
			std::vector<int> sequenceVector{ std::begin(rkSequence), std::end(rkSequence) };
			int sizeOfSequence = (int)rkSequence.size();

			/*for k in range(1, len(dsequence) + 1) :
				left = sum(dsequence[:k])
				right = k * (k - 1) + sum([min(x, k) for x in dsequence[k:]])
				if left > right:
					return False*/

			std::list<int> range(rkSequence.size());
			std::iota(range.begin(), range.end(), 1);

			for (const int k : range)
			{
				int left = std::accumulate(sequenceVector.begin(), sequenceVector.begin()+k, 0);
				std::vector<int> endSequence;
				for (std::vector<int>::iterator x = sequenceVector.begin()+k;
					x != sequenceVector.end();
					x++)
				{
					endSequence.push_back(std::min(*x, k));
				}
				int right = k * (k-1) + std::accumulate(endSequence.begin(), endSequence.end(), 0);
				if (left > right)
				{
					return false;
				}
			}
		}
		else
		{
			return false;
		}

		return true;
	}

	void Graph::RemoveVertices(const std::list<Vertex::Ptr>& rkVertices)
	{
		for (const Vertex::Ptr& kpVertex : rkVertices)
		{
			TopoDS_Vertex occtVertex = kpVertex->GetOcctVertex();

			// Check if the connected vertices are connected to the vertex to be deleted. Remove it.
			for (GraphMap::iterator graphIterator = m_graphDictionary.begin();
				graphIterator != m_graphDictionary.end();
				graphIterator++)
			{
				TopTools_MapOfShape& rOcctConnectedVertices = graphIterator->second;
				rOcctConnectedVertices.Remove(occtVertex);
			}

			// Remove the entry from this map
			GraphMap::iterator vertexIterator = m_graphDictionary.find(occtVertex);
			if (vertexIterator != m_graphDictionary.end())
			{
				m_graphDictionary.erase(vertexIterator);
			}
		}
	}

	void Graph::RemoveEdges(const std::list<Edge::Ptr>& rkEdges)
	{
		for (const Edge::Ptr& kpEdge : rkEdges)
		{
			Vertex::Ptr startVertex = kpEdge->StartVertex();
			Vertex::Ptr endVertex = kpEdge->EndVertex();

			{
				GraphMap::iterator startVertexIterator = m_graphDictionary.find(startVertex->GetOcctVertex());
				if (startVertexIterator != m_graphDictionary.end())
				{
					TopTools_MapOfShape& adjacentVertices = startVertexIterator->second;
					adjacentVertices.Remove(endVertex->GetOcctVertex());
				}
			}

			{
				GraphMap::iterator endVertexIterator = m_graphDictionary.find(endVertex->GetOcctVertex());
				if (endVertexIterator != m_graphDictionary.end())
				{
					TopTools_MapOfShape& adjacentVertices = endVertexIterator->second;
					adjacentVertices.Remove(startVertex->GetOcctVertex());
				}
			}
		}
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
			std::list<Vertex::Ptr> subtopologyVertices;
			graph->Vertices(subtopologyVertices);
			std::list<Edge::Ptr> subtopologyEdges;
			graph->Edges(subtopologyEdges);
			vertices.insert(vertices.end(), subtopologyVertices.begin(), subtopologyVertices.end());
			edges.insert(edges.end(), subtopologyEdges.begin(), subtopologyEdges.end());
		}
		return std::make_shared<Graph>(vertices, edges);
	}

	Wire::Ptr Graph::ConstructPath(const std::list<Vertex::Ptr>& rkPathVertices)
	{
		std::list<Vertex::Ptr>::const_iterator lastVertexIterator = rkPathVertices.end();
		lastVertexIterator--; 
		std::list<Edge::Ptr> edges;
		for (std::list<Vertex::Ptr>::const_iterator vertexIterator = rkPathVertices.begin();
			vertexIterator != lastVertexIterator;
			vertexIterator++)
		{
			std::list<Vertex::Ptr>::const_iterator nextVertexIterator = vertexIterator;
			nextVertexIterator++;
			Edge::Ptr edge = Edge::ByStartVertexEndVertex(*vertexIterator, *nextVertexIterator);
			edges.push_back(edge);
		}
		if (edges.empty())
		{
			return nullptr;
		}
		Wire::Ptr pathWire = Wire::ByEdges(edges);
		return pathWire;
	}

	bool Graph::IsDegreeSequence(const std::list<int>& rkSequence) const
	{
		std::list<int>::const_iterator beforeEndIterator = rkSequence.end();
		beforeEndIterator--;

		for(std::list<int>::const_iterator sequenceIterator = rkSequence.begin();
			sequenceIterator != beforeEndIterator;
			sequenceIterator++)
		{
			std::list<int>::const_iterator nextIterator = sequenceIterator;
			nextIterator++;

			if (*nextIterator > *sequenceIterator)
			{
				return false;
			}
		}

		return true;
	}

	TopoDS_Vertex Graph::GetCoincidentVertex(const TopoDS_Vertex & rkVertex, const bool kUseTolerance, const double kTolerance) const
	{
		if (kUseTolerance)
		{
			double absDistanceThreshold = std::abs(kTolerance);
			for (GraphMap::const_iterator graphIterator = m_graphDictionary.begin();
				graphIterator != m_graphDictionary.end();
				graphIterator++)
			{
				TopoDS_Vertex currentVertex = graphIterator->first;
				BRepExtrema_DistShapeShape occtDistanceCalculation(rkVertex, currentVertex);
				bool isDone = occtDistanceCalculation.Perform();
				if (isDone)
				{
					double distance = occtDistanceCalculation.Value();
					if (distance < absDistanceThreshold)
					{
						return currentVertex;
					}
				}
			}

			return TopoDS_Vertex(); // null vertex
		}
		else
		{
			GraphMap::const_iterator graphIterator = m_graphDictionary.find(rkVertex);
			if (graphIterator == m_graphDictionary.end())
			{
				return TopoDS_Vertex();
			}

			return rkVertex;
		}
	}
}