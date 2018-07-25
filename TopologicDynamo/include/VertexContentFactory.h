#pragma once

#include <ContentFactory.h>

namespace Topologic
{
	ref class VertexContentFactory : ContentFactory
	{
	public protected:
		virtual Topology^ Create(const TopologicCore::Topology::Ptr& kpTopology) override;
	};

}