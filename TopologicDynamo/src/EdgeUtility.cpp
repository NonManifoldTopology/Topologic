#include "EdgeUtility.h"
#include "Vertex.h"

namespace Topologic
{
	namespace Support
	{
		Edge ^ EdgeUtility::ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices)
		{
			std::list<TopologicCore::Vertex::Ptr> pCoreVertices;
			for each(Vertex^ pVertex in vertices)
			{
				pCoreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
			}

			try {
				TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Edge::ByVertices(pCoreVertices);
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
			TopologicCore::Edge::Ptr pCoreCircleEdge = TopologicCore::Edge::ByCircle(pCoreCentrePoint, radius, xAxisX, xAxisY, xAxisZ, normalX, normalY, normalZ);
			return gcnew Edge(pCoreCircleEdge);
		}

		double EdgeUtility::ParameterAtVertex(Edge ^ edge, Vertex ^ vertex)
		{
			TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
			try {
				return pCoreEdge->ParameterAtPoint(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery()));
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}
		}

		Vertex ^ EdgeUtility::VertexAtParameter(Edge ^ edge, double parameter)
		{
			TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
			TopologicCore::Vertex::Ptr pCoreVertex = pCoreEdge->PointAtParameter(parameter);
			return gcnew Vertex(pCoreVertex);
		}
	}
}