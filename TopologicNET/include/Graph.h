#pragma once

#include <TopologicCore/include/Graph.h>

#ifdef TOPOLOGIC_DYNAMO
using namespace Autodesk::DesignScript::Runtime;
#endif

using namespace System::Collections::Generic;

namespace Topologic {
	ref class Vertex;
	ref class Edge;
	ref class Topology;

	public ref class Graph
	{
	public:
		static Graph^ ByVerticesEdges(List<Vertex^>^ vertices, List<Edge^>^ edges);


#ifdef TOPOLOGIC_DYNAMO
		static Graph^ ByTopology(
			Topology^ topology,
			[DefaultArgument("true")] bool direct,
			[DefaultArgument("false")] bool viaSharedTopologies,
			[DefaultArgument("false")] bool viaSharedApertures,
			[DefaultArgument("false")] bool toExteriorTopologies,
			[DefaultArgument("false")] bool toExteriorApertures);
#else
		static Graph^ ByTopology(
			Topology^ topology,
			bool direct,
			bool viaSharedTopologies,
			bool viaSharedApertures,
			bool toExteriorTopologies,
			bool toExteriorApertures);
#endif

		property Topology^ Topology
		{
			Topologic::Topology^ get();
		}

	public protected:
		Graph(const std::shared_ptr<TopologicCore::Graph>& kpCoreGraph);

	protected:
		virtual ~Graph();

		std::shared_ptr<TopologicCore::Graph>* m_pCoreGraph;
	};
}