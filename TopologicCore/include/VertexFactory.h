#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class VertexFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<VertexFactory> Ptr;

	public:
		VertexFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}