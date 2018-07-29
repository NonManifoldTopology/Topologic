#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	ref class CellComplexFactory : TopologyFactory
	{
	public protected:
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) override;
	};

}