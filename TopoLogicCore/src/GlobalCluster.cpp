#include <GlobalCluster.h>

#include <assert.h>

namespace TopoLogicCore
{
	bool GlobalCluster::Add(const std::shared_ptr<Topology>& kpTopology)
	{
		bool returnValue = GetCluster()->AddTopology(kpTopology, true);
		kpTopology->SetInGlobalCluster(returnValue);

		std::string strAnalyze = GetCluster()->Analyze();
		return returnValue;
	}

	bool GlobalCluster::Remove(const std::shared_ptr<Topology>& kpTopology)
	{
		if (!kpTopology->IsInGlobalCluster())
			return false;

		bool returnValue = GetCluster()->RemoveTopology(kpTopology);
		kpTopology->SetInGlobalCluster(false);
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