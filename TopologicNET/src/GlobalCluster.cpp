#include "GlobalCluster.h"

#ifdef _DEBUG

namespace Topologic
{
	List<Topology^>^ GlobalCluster::SubTopologies(bool refresh)
	{
		if (!refresh)
			return nullptr;

		std::list<std::shared_ptr<TopologicCore::Topology>> coreTopologies;
		TopologicCore::GlobalCluster::GetInstance().SubTopologies(coreTopologies);

		List<Topology^>^ pTopologies = gcnew List<Topology^>();
		for (std::list<std::shared_ptr<TopologicCore::Topology>>::const_iterator kTopologyIterator = coreTopologies.begin();
			kTopologyIterator != coreTopologies.end();
			kTopologyIterator++)
		{
			Topology^ pTopology = Topology::ByCoreTopology(*kTopologyIterator);
			pTopologies->Add(pTopology);
		}

		return pTopologies;
	}
}

#endif