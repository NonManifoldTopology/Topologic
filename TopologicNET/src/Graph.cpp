#include "Graph.h"
#include "Vertex.h"
#include "Edge.h"
#include "Topology.h"

#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Edge.h>

using namespace System;

namespace Topologic
{
	Graph ^ Graph::ByVerticesEdges(List<Vertex^>^ vertices, List<Edge^>^ edges)
	{
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		for each(Vertex^ vertex in vertices)
		{
			TopologicCore::Vertex::Ptr pCoreVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery());
			coreVertices.push_back(pCoreVertex);
		}
		std::list<TopologicCore::Edge::Ptr> coreEdges;
		for each(Edge^ edge in edges)
		{
			TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
			coreEdges.push_back(pCoreEdge);
		}
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

	Topology^ Graph::Topology::get()
	{
		TopologicCore::Topology::Ptr coreTopology = (*m_pCoreGraph)->Topology();
		return Topologic::Topology::ByCoreTopology(coreTopology);
	}
}