#include <CellComplexFactory.h>

#include <CellComplex.h>

namespace Topologic
{
	Topology^ CellComplexFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	{
		TopologicCore::CellComplex::Ptr pCoreCellComplex = std::dynamic_pointer_cast<TopologicCore::CellComplex>(kpTopology);
		if (pCoreCellComplex == nullptr)
		{
			return nullptr;
		}
		return gcnew CellComplex(pCoreCellComplex);
	}
}