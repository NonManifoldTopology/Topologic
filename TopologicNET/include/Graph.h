#pragma once

#include <TopologicCore/include/Graph.h>

using namespace System::Collections::Generic;

namespace Topologic {
	ref class Vertex;
	ref class Edge;

	public ref class Graph
	{
	public:
		static Graph^ ByVerticesEdges(List<Vertex^>^ vertices, List<Edge^>^ edges);

	public protected:
		Graph(const std::shared_ptr<TopologicCore::Graph>& kpCoreGraph);

	protected:
		virtual ~Graph();

		std::shared_ptr<TopologicCore::Graph>* m_pCoreGraph;
	};
}