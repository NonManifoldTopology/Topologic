#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class ClusterFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<ClusterFactory> Ptr;

	public:
		ClusterFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}