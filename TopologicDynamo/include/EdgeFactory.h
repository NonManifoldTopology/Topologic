#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	/// <summary>
	/// (private) A private factory class to create an Edge.
	/// </summary>
	ref class EdgeFactory : TopologyFactory
	{
	public:
		/// <summary>
		/// Creates an Edge from a TopologicCore layer Topology.
		/// </summary>
		/// <param name="kpTopology">A TopologicCore layer Topology</param>
		/// <returns name="Topology">The created Edge</returns>
		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};
}