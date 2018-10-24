#include "GlobalCluster.h"

#ifdef _DEBUG

namespace Topologic
{
	List<Topology^>^ GlobalCluster::SubTopologies(Topology^ topology)
	{
		List<Topology^>^ pTopologies = gcnew List<Topology^>();
		TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());

		std::list<std::shared_ptr<TopologicCore::Topology>> coreTopologies;
		pCoreTopology->GlobalClusterSubTopologies(coreTopologies);

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