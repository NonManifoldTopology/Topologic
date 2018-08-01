#include <CellComplexFactory.h>

#include <CellComplex.h>

namespace Topologic
{
	//Topology^ CellComplexFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	//{
	//	TopologicCore::CellComplex::Ptr pCoreCellComplex = std::dynamic_pointer_cast<TopologicCore::CellComplex>(kpTopology);
	//	if (pCoreCellComplex == nullptr)
	//	{
	//		return nullptr;
	//	}
	//	return gcnew CellComplex(pCoreCellComplex);
	//}
	//Topology ^ CellComplexFactory::Create(TopologicCore::Topology const * const kpTopology)
	//{
	//	throw gcnew System::NotImplementedException();
	//	// TODO: insert return statement here
	//}
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