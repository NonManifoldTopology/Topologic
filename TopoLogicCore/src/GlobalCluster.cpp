#include <GlobalCluster.h>

#include <assert.h>

namespace TopoLogicCore
{
	bool GlobalCluster::Add(Topology * const kpTopology)
	{
		bool returnValue = GetCluster()->AddTopology(kpTopology, true);
		kpTopology->SetInGlobalCluster(returnValue);

		return returnValue;
	}

	bool GlobalCluster::Remove(Topology* pTopology)
	{
		if (!pTopology->IsInGlobalCluster())
			return false;

		bool returnValue = GetCluster()->RemoveTopology(pTopology);
		pTopology->SetInGlobalCluster(false);
		return returnValue;
	}

	std::shared_ptr<Cluster> GlobalCluster::GetCluster() const
	{
		assert(m_pCluster != nullptr && "GlobalCluster::m_pCluster is null.");
		if (m_pCluster == nullptr)
		{
			throw std::exception("GlobalCluster::m_pCluster is null.");
		}

		return m_pCluster;
	}

	GlobalCluster::GlobalCluster()
		: m_pCluster(new Cluster(false))
	{
	}

	GlobalCluster::~GlobalCluster()
	{

	}
}