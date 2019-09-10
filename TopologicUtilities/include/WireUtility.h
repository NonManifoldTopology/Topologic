#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Wire.h>
#include <TopologicCore/include/Shell.h>
#include <TopologicCore/include/Cell.h>

#include <memory>

namespace TopologicUtilities
{
	class WireUtility
	{
	public:
		typedef std::shared_ptr<WireUtility> Ptr;

	public:
		static TOPOLOGIC_UTILITIES_API void AdjacentShells(
			const TopologicCore::Wire::Ptr & kpWire,
			const TopologicCore::Topology::Ptr & kpParentTopology,
			std::list<TopologicCore::Shell::Ptr>& rCoreAdjacentShells);

		static TOPOLOGIC_UTILITIES_API void AdjacentCells(
			const TopologicCore::Wire::Ptr & kpWire,
			const TopologicCore::Topology::Ptr & kpParentTopology,
			std::list<TopologicCore::Cell::Ptr>& rCoreAdjacentCells);
	};
}