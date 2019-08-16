#pragma once

#include "TopologyFactory.h"

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A factory class to create a Cluster.
		/// </summary>
		ref class ClusterFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates a Cluster from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Cluster</returns>
#ifdef TOPOLOGIC_DYNAMO
			[IsVisibleInDynamoLibrary(false)]
#endif
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}