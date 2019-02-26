#pragma once

#include "Utilities.h"
#include "Vertex.h"
#include "Edge.h"

#include <list>

namespace TopologicCore
{
	class Vertex;
	class Edge;

	class Graph
	{
	public:
		typedef std::shared_ptr<Graph> Ptr;

	public:
		static TOPOLOGIC_API Graph::Ptr ByVerticesEdges(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges);

		Graph(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges);

		virtual ~Graph();

	protected:
		std::list<Vertex::Ptr> m_vertices;
		std::list<Edge::Ptr> m_edges;

		std::map<TopoDS_Vertex, std::list<TopoDS_Edge>> m_graphDictionary;
	};
}