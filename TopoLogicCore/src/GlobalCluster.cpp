#include <GlobalCluster.h>

namespace TopoLogicCore
{
	void GlobalCluster::Add(Topology const * const kpkTopology)
	{
	}
	void GlobalCluster::Remove(Topology * kpkTopology)
	{
	}
	GlobalCluster::GlobalCluster()
	{
	}
	GlobalCluster::~GlobalCluster()
	{
		delete m_pCluster;
	}
}