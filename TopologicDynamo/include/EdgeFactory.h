#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	ref class EdgeFactory : TopologyFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};
}