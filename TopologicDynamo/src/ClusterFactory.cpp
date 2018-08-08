#include <ClusterFactory.h>

#include <Cluster.h>

namespace Topologic
{
	Topology ^ ClusterFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
	{
		TopologicCore::Cluster::Ptr pCoreCluster = std::dynamic_pointer_cast<TopologicCore::Cluster>(kpTopology.topologyPtr);
		if (pCoreCluster == nullptr)
		{
			return nullptr;
		}
		return gcnew Cluster(pCoreCluster);
	}
}