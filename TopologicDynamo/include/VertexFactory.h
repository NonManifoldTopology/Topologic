#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	ref class VertexFactory : TopologyFactory
	{
	public protected:
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) override;
	};

}