#pragma once

#include "TopologyFactory.h"

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A private factory class to create a Shell.
		/// </summary>
		ref class ShellFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates a Shell from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Shell</returns>
			[IsVisibleInDynamoLibrary(false)]
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}