#pragma once

#include <TopologyFactory.h>

namespace Topologic
{
	ref class FaceFactory : TopologyFactory
	{
	public:
		/*[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) override;

		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(TopologicCore::Topology const * const kpTopology) override;*/

		[IsVisibleInDynamoLibrary(false)]
		virtual Topology^ Create(const TopologicCore::TopologyPtr& kpTopology) override;
	};

}