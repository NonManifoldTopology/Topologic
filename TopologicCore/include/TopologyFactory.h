#pragma once

#include <TopoDS_Shape.hxx>

#include <memory>

namespace TopologicCore
{
	class Topology;

	class TopologyFactory
	{
	public:
		typedef std::shared_ptr<TopologyFactory> Ptr;

	public:
		TopologyFactory() {}

		virtual std::shared_ptr<Topology> Create(const TopoDS_Shape& rkOcctShape) = 0;
	};
}