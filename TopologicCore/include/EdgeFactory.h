#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class EdgeFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<EdgeFactory> Ptr;

	public:
		EdgeFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}