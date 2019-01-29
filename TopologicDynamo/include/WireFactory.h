#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A private factory class to create a Wire.
		/// </summary>
		ref class WireFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates a Wire from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Wire</returns>
			[IsVisibleInDynamoLibrary(false)]
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}