#include "WireUtility.h"
#include "Wire.h"
#include "Shell.h"
#include "Cell.h"

#include <TopologicUtilities/include/WireUtility.h>

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