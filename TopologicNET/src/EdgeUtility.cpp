#include "EdgeUtility.h"
#include "Vertex.h"

#include <TopologicUtilities/include/EdgeUtility.h>

namespace Topologic
{
	namespace Utilities
	{
		Edge ^ EdgeUtility::ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices)
		{
			std::list<TopologicCore::Vertex::Ptr> pCoreVertices;
			for each(Vertex^ pVertex in vertices)
			{
				pCoreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
			}

			try {
				TopologicCore::Edge::Ptr pCoreEdge = TopologicUtilities::EdgeUtility::ByVertices(pCoreVertices);
				return gcnew Edge(pCoreEdge);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}
		}

		Edge ^ EdgeUtility::ByCircle(Vertex ^ centerPoint, double radius, double xAxisX, double xAxisY, double xAxisZ, double normalX, double normalY, double normalZ)
		{
			TopologicCore::Vertex::Ptr pCoreCentrePoint = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(centerPoint->GetCoreTopologicalQuery());
			TopologicCore::Edge::Ptr pCoreCircleEdge = TopologicUtilities::EdgeUtility::ByCircle(pCoreCentrePoint, radius, xAxisX, xAxisY, xAxisZ, normalX, normalY, normalZ);
			return gcnew Edge(pCoreCircleEdge);
		}

		double EdgeUtility::ParameterAtVertex(Edge ^ edge, Vertex ^ vertex)
		{
			TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
			try {
				return TopologicUtilities::EdgeUtility::ParameterAtPoint(pCoreEdge, TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery()));
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}
		}

		Vertex ^ EdgeUtility::VertexAtParameter(Edge ^ edge, double u)
		{
			TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreVertex = TopologicUtilities::EdgeUtility::PointAtParameter(pCoreEdge, u);
			return gcnew Vertex(pCoreVertex);
		}
	}
}