#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class CellFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<CellFactory> Ptr;

	public:
		CellFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}