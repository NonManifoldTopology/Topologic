#include "Graph.h"
#include "Cluster.h"

#include <BRepExtrema_DistShapeShape.hxx>

namespace TopologicCore
{
	Graph::Ptr Graph::ByVerticesEdges(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges)
	{
		return std::make_shared<Graph>(rkVertices, rkEdges);
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
				BRepExtrema_DistShapeShape occtDistanceCalculation(rkOcctVertex, rkOcctExistingVertex);
				bool isDone = occtDistanceCalculation.Perform();
				if (isDone)
				{
					double distance = occtDistanceCalculation.Value();
					if (distance < 0.0001)
					{
						isAdded = true;
						break;
					}
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
					m_vertices.push_back(vertex);
				}
			} // for (const Vertex::Ptr& kpVertex : vertices)
		}
	}

	Graph::~Graph()
	{
		m_graphDictionary.clear();
		m_edges.clear();
		m_vertices.clear();
	}

	Topology::Ptr Graph::Topology()
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
}