#pragma once

#include "TopologyFactory.h"

namespace Topologic
{
	namespace Factories
	{
		/// <summary>
		/// (private) A private factory class to create an Aperture.
		/// </summary>
		ref class ApertureFactory : TopologyFactory
		{
		public:
			/// <summary>
			/// Creates an Aperture from a TopologicCore layer Topology.
			/// </summary>
			/// <param name="kpTopology">A TopologicCore layer Topology</param>
			/// <returns name="Topology">The created Aperture</returns>
			[IsVisibleInDynamoLibrary(false)]
			virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
		};
	}
}