#pragma once

#include <TopologyFactory.h>

namespace TopologicCore
{
	class CellComplexFactory : public TopologyFactory
	{
	public:
		typedef std::shared_ptr<CellComplexFactory> Ptr;

	public:
		CellComplexFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape);
	};
}