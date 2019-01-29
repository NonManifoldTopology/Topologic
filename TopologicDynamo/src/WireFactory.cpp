#include <WireFactory.h>

#include <Wire.h>

namespace Topologic
{
	namespace Factories
	{
		Topology ^ WireFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicCore::Wire::Ptr pCoreWire = std::dynamic_pointer_cast<TopologicCore::Wire>(kpTopology.topologyPtr);
			if (pCoreWire == nullptr)
			{
				return nullptr;
			}
			return gcnew Wire(pCoreWire);
		}
	}
}