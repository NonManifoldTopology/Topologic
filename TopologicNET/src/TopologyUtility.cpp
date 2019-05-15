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

			TopologicCore::Topology::Ptr pCoreTranslatedTopology = TopologicUtilities::TopologyUtility::Translate(pCoreTopology, x, y, z);

			return Topology::ByCoreTopology(pCoreTranslatedTopology);
		}

		Topology ^ TopologyUtility::Rotate(Topology ^ topology, 
			Vertex^ origin,
			double xVector, double yVector, double zVector,
			double degree)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreOrigin =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(origin->GetCoreTopologicalQuery());

			TopologicCore::Topology::Ptr pCoreRotatedTopology = TopologicUtilities::TopologyUtility::Rotate(pCoreTopology, pCoreOrigin, xVector, yVector, zVector, degree);

			return Topology::ByCoreTopology(pCoreRotatedTopology);
		}

		Topology ^ TopologyUtility::Scale(Topology ^ topology, Vertex ^ origin, double xFactor, double yFactor, double zFactor)
		{
			// 1. Copy this topology
			TopologicCore::Topology::Ptr pCoreTopology =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreOrigin =
				TopologicCore::TopologicalQuery::Downcast<TopologicCore::Vertex>(origin->GetCoreTopologicalQuery());

			TopologicCore::Topology::Ptr pCoreScaledTopology = TopologicUtilities::TopologyUtility::Scale(pCoreTopology, pCoreOrigin, xFactor, yFactor, zFactor);

			return Topology::ByCoreTopology(pCoreScaledTopology);
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

		Topology ^ TopologyUtility::Transform(Topology ^ topology,
			double translationX, double translationY, double translationZ,
			double rotation11, double rotation12, double rotation13,
			double rotation21, double rotation22, double rotation23,
			double rotation31, double rotation32, double rotation33)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();

			TopologicCore::Topology::Ptr pCoreTransformedTopology =
				TopologicUtilities::TopologyUtility::Transform(
					pCoreTopology, 
					translationX, translationY, translationZ, 
					rotation11, rotation12, rotation13,
					rotation21, rotation22, rotation23,
					rotation31, rotation32, rotation33);

			return Topology::ByCoreTopology(pCoreTransformedTopology);
		}

		Topology ^ TopologyUtility::Transform(Topology ^ topology,
			double zAxisX, double zAxisY, double zAxisZ,
			double xAxisX, double xAxisY, double xAxisZ)
		{
			TopologicCore::Topology::Ptr pCoreTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreCenterOfMass = pCoreTopology->CenterOfMass();

			TopologicCore::Topology::Ptr pCoreTransformedTopology =
					TopologicUtilities::TopologyUtility::Transform(
						pCoreTopology, pCoreCenterOfMass,
						zAxisX, zAxisY, zAxisZ,
						xAxisX, xAxisY, xAxisZ);

			return Topology::ByCoreTopology(pCoreTransformedTopology);
		}
	}
}