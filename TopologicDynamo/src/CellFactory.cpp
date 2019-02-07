#include "CellFactory.h"
#include "Cell.h"

namespace Topologic
{
	namespace Factories
	{
		Topology ^ CellFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicCore::Cell::Ptr pCoreCell = std::dynamic_pointer_cast<TopologicCore::Cell>(kpTopology.topologyPtr);
			if (pCoreCell == nullptr)
			{
				return nullptr;
			}
			return gcnew Cell(pCoreCell);
		}
	}
}