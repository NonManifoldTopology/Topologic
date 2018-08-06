#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class DualGraphFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<DualGraphFactory> Ptr;

	public:
		DualGraphFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}