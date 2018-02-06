#include <GlobalCluster.h>

#include <assert.h>

namespace TopoLogicCore
{
	bool GlobalCluster::Add(Topology const * const kpkTopology)
	{
		return GetCluster()->Add(kpkTopology);
	}

	bool GlobalCluster::Remove(Topology * kpkTopology)
	{
		return GetCluster()->Remove(kpkTopology);
	}

	Cluster* GlobalCluster::GetCluster() const
	{
		assert(m_pCluster != nullptr && "GlobalCluster::m_pCluster is null.");
		if (m_pCluster == nullptr)
		{
			throw std::exception("GlobalCluster::m_pCluster is null.");
		}

		return m_pCluster;
	}

	GlobalCluster::GlobalCluster()
		: m_pCluster(new Cluster(new TopoDS_Compound(), false))
	{
	}

	GlobalCluster::~GlobalCluster()
	{
		delete m_pCluster;
	}
}