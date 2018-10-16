#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	ref class GraphFactory : TopologyFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};
}