#pragma once

#include "TopologyFactory.h"

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A factory class to create a Vertex.
		/// </summary>
		ref class VertexFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates a Vertex from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Vertex</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}