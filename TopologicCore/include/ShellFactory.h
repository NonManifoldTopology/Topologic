#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class ShellFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<ShellFactory> Ptr;

	public:
		ShellFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}