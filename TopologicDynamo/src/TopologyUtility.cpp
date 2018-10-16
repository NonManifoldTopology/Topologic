#include "TopologyUtility.h"
#include "Vertex.h"

namespace Topologic
{
	namespace Support
	{
		Topology^ TopologyUtility::Translate(Topology^ topology, double x, double y, double z)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->Copy();
			pCoreCopyTopology->Translate(x, y, z);

			return Topology::ByCoreTopology(pCoreCopyTopology);
		}

		double TopologyUtility::Distance(Topology ^ topology, Topology ^ anotherTopology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreOtherTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(anotherTopology->GetCoreTopologicalQuery());

			return pCoreTopology->Distance(pCoreOtherTopology);
		}

		Vertex^ TopologyUtility::CenterOfMass(Topology^ topology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();
			return gcnew Vertex(pCoreCenterOfMass);
		}

	}
}