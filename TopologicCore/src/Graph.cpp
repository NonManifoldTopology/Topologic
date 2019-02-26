#include "Graph.h"

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
		for (const Vertex::Ptr& kpVertex : rkVertices)
		{
			const TopoDS_Vertex& rkOcctVertex = kpVertex->GetOcctVertex();

			// 1. Geometric check of the edges
			for (const std::pair<TopoDS_Vertex, std::list<TopoDS_Edge>>& kpPair : m_graphDictionary)
			{
				const TopoDS_Vertex& rkOcctExistingVertex = kpPair.first;
				BRepExtrema_DistShapeShape occtDistanceCalculation(rkOcctVertex, rkOcctExistingVertex);
				bool isDone = occtDistanceCalculation.Perform();
				if (isDone)
				{
					double distance = occtDistanceCalculation.Value();
					if (distance < 0.0001)
					{
						break;
					}
				}
			}
		}
	}

	Graph::~Graph()
	{
		m_graphDictionary.clear();
		m_edges.clear();
		m_vertices.clear();
	}
}