#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class GraphFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<GraphFactory> Ptr;

	public:
		GraphFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}