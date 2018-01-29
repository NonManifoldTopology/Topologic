#include <Cluster.h>

namespace TopoLogicCore
{
	Cluster * Cluster::ByTopology(const std::list<Topology*>& rkTopologies)
	{
		return nullptr;
	}
	Cluster::Cluster(TopoDS_Compound * const kpOcctCompound)
		: Topology(3)
		, m_pOcctCompound(kpOcctCompound)
	{
	}
	Cluster::~Cluster()
	{
	}
}
