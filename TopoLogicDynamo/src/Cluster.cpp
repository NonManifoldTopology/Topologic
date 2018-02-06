#include <Cluster.h>

#include <assert.h>

namespace TopoLogic
{
	Cluster^ Cluster::ByTopology(List<Topology^>^ topology)
	{
		std::list<TopoLogicCore::Topology*> coreTopologies;
		for each(Topology^ pTopology in topology)
		{
			coreTopologies.push_back(pTopology->GetCoreTopology());
		}
		
		return gcnew Cluster(TopoLogicCore::Cluster::ByTopology(coreTopologies));
	}

	Cluster^ Cluster::Add(Cluster^ cluster, Topology^ topology)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(cluster->GetCoreTopology());
		pCoreCluster->Add(topology->GetCoreTopology());
		return cluster;
	}

	Cluster^ Cluster::Remove(Cluster^ cluster, Topology^ topology)
	{
		TopoLogicCore::Cluster* pCoreCluster = TopoLogicCore::Topology::Downcast<TopoLogicCore::Cluster>(cluster->GetCoreTopology());
		pCoreCluster->Remove(topology->GetCoreTopology());
		return cluster;
	}

	Object^ Cluster::Geometry::get()
	{
		//throw gcnew System::NotImplementedException();
		return nullptr;
	}

	Cluster::Cluster(TopoLogicCore::Cluster* const kpCoreCluster)
		: Topology()
		, m_pCoreCluster(kpCoreCluster)
	{

	}

	TopoLogicCore::Topology* Cluster::GetCoreTopology()
	{
		assert(m_pCoreCluster != nullptr && "Cluster::m_pCoreCluster is null.");
		if (m_pCoreCluster == nullptr)
		{
			throw gcnew Exception("Cluster::m_pCoreCluster is null.");
		}

		return m_pCoreCluster;
	}

	Cluster::~Cluster()
	{
		delete m_pCoreCluster;
	}
}