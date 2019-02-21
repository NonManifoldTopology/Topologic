#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class WireFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<WireFactory> Ptr;

	public:
		WireFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}