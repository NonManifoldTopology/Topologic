#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class FaceFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<FaceFactory> Ptr;

	public:
		FaceFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}