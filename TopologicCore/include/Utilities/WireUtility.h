// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

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