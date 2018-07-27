#pragma once

#include <ContentFactory.h>

namespace Topologic
{
	ref class FaceContentFactory : ContentFactory
	{
	public protected:
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) override;
	};

}