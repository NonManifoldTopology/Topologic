#pragma once

#include <Wire.h>
#include <Shell.h>
#include <Cell.h>

#include <memory>

namespace TopologicUtilities
{
	class WireUtility
	{
	public:
		typedef std::shared_ptr<WireUtility> Ptr;

	public:
		static TOPOLOGIC_API void AdjacentShells(
			const TopologicCore::Wire::Ptr & kpWire,
			const TopologicCore::Topology::Ptr & kpParentTopology,
			std::list<TopologicCore::Shell::Ptr>& rCoreAdjacentShells);

		static TOPOLOGIC_API void AdjacentCells(
			const TopologicCore::Wire::Ptr & kpWire,
			const TopologicCore::Topology::Ptr & kpParentTopology,
			std::list<TopologicCore::Cell::Ptr>& rCoreAdjacentCells);

		static TOPOLOGIC_API TopologicCore::Wire::Ptr RemoveCollinearEdges(
			const TopologicCore::Wire::Ptr & kpWire,
			const double kTolerance
		);
	};
}