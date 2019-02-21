#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A private factory class to create a DualGraph.
		/// </summary>
		ref class DualGraphFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates a DualGraph_ from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created DualGraph_</returns>
			[IsVisibleInDynamoLibrary(false)]
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}