#pragma once

#include <TopologicCore/include/TopologyFactory.h>

namespace TopologicExtensions
{
	class GraphFactory : public TopologicCore::TopologyFactory
	{
	public:
		typedef std::shared_ptr<GraphFactory> Ptr;

	public:
		GraphFactory() {}

		virtual std::shared_ptr<TopologicCore::Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}