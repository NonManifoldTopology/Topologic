#include "TopologyUtility.h"
#include "Vertex.h"

#include <TopologicUtilities/include/TopologyUtility.h>

namespace Topologic
{
	namespace Utilities
	{
		Topology^ TopologyUtility::Translate(Topology^ topology, double x, double y, double z)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			//TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->DeepCopy();
			TopologicUtilities::TopologyUtility::Translate(pCoreTopology, x, y, z);

			return Topology::ByCoreTopology(pCoreTopology);
		}

		Topology ^ TopologyUtility::Rotate(Topology ^ topology, double pointX, double pointY, double pointZ, double directionX, double directionY, double directionZ, double angleInRadian)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			//TopologicCore::Topology::Ptr pCoreCopyTopology = pCoreTopology->DeepCopy();
			TopologicUtilities::TopologyUtility::Rotate(pCoreTopology, pointX, pointY, pointZ, directionX, directionY, directionZ, angleInRadian);

			return Topology::ByCoreTopology(pCoreTopology);
		}

		Topology ^ TopologyUtility::Scale(Topology ^ topology, Vertex ^ vertex, double scaleFactor)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreVertex =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery());
			TopologicUtilities::TopologyUtility::Scale(pCoreTopology, pCoreVertex, scaleFactor);

			return Topology::ByCoreTopology(pCoreTopology);
		}

		double TopologyUtility::Distance(Topology ^ topology, Topology ^ anotherTopology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreOtherTopology = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(anotherTopology->GetCoreTopologicalQuery());

			return TopologicUtilities::TopologyUtility::Distance(pCoreTopology, pCoreOtherTopology);
		}

		Vertex^ TopologyUtility::CenterOfMass(Topology^ topology)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();
			return gcnew Vertex(pCoreCenterOfMass);
		}

	}
}