#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	/// <summary>
	/// (private) A private factory class to create a Face.
	/// </summary>
	ref class FaceFactory : TopologyFactory
	{
	public:
		/// <summary>
		/// Creates a Face from a TopologicCore layer Topology.
		/// </summary>
		/// <param name="kpTopology">A TopologicCore layer Topology</param>
		/// <returns name="Topology">The created Face</returns>
		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};

}