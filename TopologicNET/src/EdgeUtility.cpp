// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include "EdgeUtility.h"
#include "Vertex.h"
#include "Wire.h"

#include <TopologicCore/include/Utilities/EdgeUtility.h>

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

		Edge ^ EdgeUtility::ByEllipse(Vertex ^ centerPoint, double majorRadius, double minorRadius, double xAxisX, double xAxisY, double xAxisZ, double normalX, double normalY, double normalZ)
		{
			TopologicCore::Vertex::Ptr pCoreCentrePoint = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(centerPoint->GetCoreTopologicalQuery());
			TopologicCore::Edge::Ptr pCoreEllipseEdge = TopologicUtilities::EdgeUtility::ByEllipse(pCoreCentrePoint, majorRadius, minorRadius, xAxisX, xAxisY, xAxisZ, normalX, normalY, normalZ);
			return gcnew Edge(pCoreEllipseEdge);
		}

		Edge ^ EdgeUtility::ByNurbsCurve(List<Vertex^>^ controlPoints, List<double>^ knots, List<double>^ weights, int degree, bool isPeriodic, bool isRational)
		{
			std::list<TopologicCore::Vertex::Ptr> coreVertices;
			for each (Vertex^ controlPoint in controlPoints)
			{
				TopologicCore::Vertex::Ptr pCoreControlPoint = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(controlPoint->GetCoreTopologicalQuery());
				coreVertices.push_back(pCoreControlPoint);
			}

			std::list<double> coreKnots;
			for each(double knot in knots)
			{
				coreKnots.push_back(knot);
			}

			std::list<double> coreWeights;
			for each(double weight in weights)
			{
				coreWeights.push_back(weight);
			}
			
			TopologicCore::Edge::Ptr pCoreEdge = TopologicUtilities::EdgeUtility::ByNurbsCurve(coreVertices, coreKnots, coreWeights, degree, isPeriodic, isRational);
			return gcnew Edge(pCoreEdge);
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

		List<Wire^>^ EdgeUtility::AdjacentWires(Edge ^ edge, Topology ^ parentTopology)
		{
			TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
			TopologicCore::Topology::Ptr pCoreParentTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(parentTopology->GetCoreTopologicalQuery());

			std::list<TopologicCore::Wire::Ptr> coreAdjacentWires;
			try {
				TopologicUtilities::EdgeUtility::AdjacentWires(pCoreEdge, pCoreParentTopology, coreAdjacentWires);
			}
			catch (const std::exception& rkException)
			{
				throw gcnew Exception(gcnew String(rkException.what()));
			}

			List<Wire^>^ adjacentWires = gcnew List<Wire^>();
			for (std::list<TopologicCore::Wire::Ptr>::const_iterator kAdjacentWireIterator = coreAdjacentWires.begin();
				kAdjacentWireIterator != coreAdjacentWires.end();
				kAdjacentWireIterator++)
			{
				Wire^ wire = gcnew Wire(*kAdjacentWireIterator);
				adjacentWires->Add(wire);
			}

			return adjacentWires;
		}
	}
}