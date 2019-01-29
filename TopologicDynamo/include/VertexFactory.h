#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A private factory class to create a Vertex.
		/// </summary>
		ref class VertexFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates a Vertex from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Vertex</returns>
			[IsVisibleInDynamoLibrary(false)]
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}