#include "Graph.h"

#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Edge.h>

using namespace System;

namespace Topologic
{
	Graph ^ Graph::ByVerticesEdges(List<Vertex^>^ vertices, List<Edge^>^ edges)
	{
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		TopologicCore::Graph::Ptr pCoreGraph = TopologicCore::Graph::ByVerticesEdges(coreVertices, coreEdges);
		return gcnew Graph(pCoreGraph);
	}

	Graph::Graph(const std::shared_ptr<TopologicCore::Graph>& kpCoreGraph)
		: m_pCoreGraph(kpCoreGraph != nullptr ? new TopologicCore::Graph::Ptr(kpCoreGraph) : throw gcnew Exception("A null graph was created."))
	{

	}

	Graph::~Graph()
	{
		delete m_pCoreGraph;
	}
}