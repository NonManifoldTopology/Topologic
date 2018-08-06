#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class ApertureFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<ApertureFactory> Ptr;

	public:
		ApertureFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}