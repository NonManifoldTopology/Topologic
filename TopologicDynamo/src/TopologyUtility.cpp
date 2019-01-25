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

		Topology ^ TopologyUtility::Rotate(Topology ^ topology, 
			Vertex^ origin,
			Autodesk::DesignScript::Geometry::Vector^ vector,
			double degree)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreOrigin =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(origin->GetCoreTopologicalQuery());
			TopologicUtilities::TopologyUtility::Rotate(pCoreTopology, pCoreOrigin, vector->X, vector->Y, vector->Z, degree);

			return Topology::ByCoreTopology(pCoreTopology);
		}

		Topology ^ TopologyUtility::Scale(Topology ^ topology, Vertex ^ origin, double xFactor, double yFactor, double zFactor)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreOrigin =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(origin->GetCoreTopologicalQuery());
			TopologicUtilities::TopologyUtility::Scale(pCoreTopology, pCoreOrigin, xFactor, yFactor, zFactor);

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