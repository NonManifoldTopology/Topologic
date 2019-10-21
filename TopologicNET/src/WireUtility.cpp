// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "WireUtility.h"
#include "Wire.h"
#include "Shell.h"
#include "Cell.h"

#include <TopologicCore/include/Utilities/WireUtility.h>

namespace Topologic {
	namespace Utilities {
		List<Shell^>^ WireUtility::AdjacentShells(Wire ^ wire, Topology ^ parentTopology)
		{
			TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(wire->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Shell::Ptr> coreAdjacentShells;
			try {
				TopologicUtilities::WireUtility::AdjacentShells(pCoreWire, pCoreParentTopology, coreAdjacentShells);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}

			List<Shell^>^ adjacentShells = gcnew List<Shell^>();
			for (std::list<TopologicCore::Shell::Ptr>::const_iterator kAdjacentShellIterator = coreAdjacentShells.begin();
				kAdjacentShellIterator != coreAdjacentShells.end();
				kAdjacentShellIterator++)
			{
				Shell^ shell = gcnew Shell(*kAdjacentShellIterator);
				adjacentShells->Add(shell);
			}

			return adjacentShells;
		}

		List<Cell^>^ WireUtility::AdjacentCells(Wire ^ wire, Topology ^ parentTopology)
		{
			TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(wire->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Cell::Ptr> coreAdjacentCells;
			try {
				TopologicUtilities::WireUtility::AdjacentCells(pCoreWire, pCoreParentTopology, coreAdjacentCells);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}

			List<Cell^>^ adjacentCells = gcnew List<Cell^>();
			for (std::list<TopologicCore::Cell::Ptr>::const_iterator kAdjacentCellIterator = coreAdjacentCells.begin();
				kAdjacentCellIterator != coreAdjacentCells.end();
				kAdjacentCellIterator++)
			{
				Cell^ cell = gcnew Cell(*kAdjacentCellIterator);
				adjacentCells->Add(cell);
			}

			return adjacentCells;
		}

		Wire ^ WireUtility::RemoveCollinearEdges(Wire ^ wire, double tolerance)
		{
			TopologicCore::Wire::Ptr pCoreWire = TopologicCore::Topology::Downcast<TopologicCore::Wire>(wire->GetCoreTopologicalQuery());
			
			try {
				TopologicCore::Wire::Ptr pCoreNewWire = TopologicUtilities::WireUtility::RemoveCollinearEdges(pCoreWire, tolerance);
				return gcnew Wire(pCoreNewWire);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}
		}
	}
}