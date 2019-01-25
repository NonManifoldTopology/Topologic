#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	/// <summary>
	/// (private) A private factory class to create a CellComplex.
	/// </summary>
	ref class CellComplexFactory : TopologyFactory
	{
	public:
		/// <summary>
		/// Creates a CellComplex from a TopologicCore layer Topology.
		/// </summary>
		/// <param name="kpTopology">A TopologicCore layer Topology</param>
		/// <returns name="Topology">The created CellComplex</returns>
		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};

}