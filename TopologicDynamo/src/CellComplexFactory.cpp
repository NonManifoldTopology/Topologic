#include <CellComplexFactory.h>

#include <CellComplex.h>

namespace Topologic
{
	namespace Factories
	{
		Topology ^ CellComplexFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicCore::CellComplex::Ptr pCoreCellComplex = std::dynamic_pointer_cast<TopologicCore::CellComplex>(kpTopology.topologyPtr);
			if (pCoreCellComplex == nullptr)
			{
				return nullptr;
			}
			return gcnew CellComplex(pCoreCellComplex);
		}
	}
}