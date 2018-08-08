#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	ref class ApertureFactory : TopologyFactory
	{
	public:
		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};

}