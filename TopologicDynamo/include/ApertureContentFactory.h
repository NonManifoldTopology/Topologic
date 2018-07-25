#pragma once

#include <ContentFactory.h>

namespace Topologic
{
	ref class ApertureContentFactory : ContentFactory
	{
	public protected:
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) override;
	};

}