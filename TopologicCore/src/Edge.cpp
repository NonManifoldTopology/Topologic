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
// along with this program. If not, see <https://www.gnu.org/licenses/>.

#include "Edge.h"
#include "Vertex.h"
#include "Wire.h"
#include "Face.h"
#include "EdgeFactory.h"
#include "GlobalCluster.h"
#include "AttributeManager.h"

#include <BRepGProp.hxx>
#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GProp_GProps.hxx>
#include <gp_Lin.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <ShapeFix_Shape.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Edge::AdjacentEdges(std::list<Edge::Ptr>& rAdjacentEdges) const
	{
		std::list<Vertex::Ptr> vertices;
		Vertices(vertices);

		// Find the constituent edges
		TopTools_MapOfShape occtAdjacentEdges;
		for (const Vertex::Ptr& kpVertex : vertices)
		{
			// Find the edges
			std::list<Edge::Ptr> edges;
			kpVertex->Edges(edges);

			for (const Edge::Ptr& kpEdge : edges)
			{
				// If not the same as this Edge, add to the list
				if (!IsSame(kpEdge))
				{
					occtAdjacentEdges.Add(kpEdge->GetOcctShape());
				}
			}
		}

		for (TopTools_MapIteratorOfMapOfShape occtAdjacentEdgeIterator(occtAdjacentEdges);
			occtAdjacentEdgeIterator.More();
			occtAdjacentEdgeIterator.Next())
		{
			rAdjacentEdges.push_back(std::make_shared<Edge>(TopoDS::Edge(occtAdjacentEdgeIterator.Value())));
		}
	}

	std::shared_ptr<Vertex> Edge::StartVertex() const
	{
		return std::make_shared<Vertex>(StartVertex(GetOcctEdge()));
	}

	TopoDS_Vertex Edge::StartVertex(const TopoDS_Edge & rkOcctEdge)
	{
		ShapeAnalysis_Edge occtShapeAnalysisEdge;
		TopoDS_Vertex occtFirstVertex = occtShapeAnalysisEdge.FirstVertex(rkOcctEdge);
		return occtFirstVertex;
	}

	TopoDS_Vertex Edge::EndVertex(const TopoDS_Edge & rkOcctEdge)
	{
		ShapeAnalysis_Edge occtShapeAnalysisEdge;
		TopoDS_Vertex occtLastVertex = occtShapeAnalysisEdge.LastVertex(rkOcctEdge);
		return occtLastVertex;
	}

	std::shared_ptr<Vertex> Edge::EndVertex() const
	{
		return std::make_shared<Vertex>(EndVertex(GetOcctEdge()));
	}

	void Edge::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		rVertices.push_back(StartVertex());
		rVertices.push_back(EndVertex());
	}

	void Edge::Wires(std::list<Wire::Ptr>& rWires) const
	{
		UpwardNavigation(rWires);
	}

	void Edge::Faces(std::list<std::shared_ptr<Face>>& rFaces) const
	{
		UpwardNavigation(rFaces);
	}

	Edge::Ptr Edge::ByCurve(
		const TColgp_Array1OfPnt &rkOcctPoles,
		const TColStd_Array1OfReal &rkOcctWeights,
		const TColStd_Array1OfReal &rkOcctKnots,
		const TColStd_Array1OfInteger &rkOcctMultiplicities,
		const int kDegree,
		const bool kIsPeriodic,
		const bool kIsRational)
	{
		BRepBuilderAPI_MakeEdge occtMakeEdge;
		Handle(Geom_BSplineCurve) pOcctBSplineCurve = nullptr;
		try {
			pOcctBSplineCurve = new Geom_BSplineCurve(
				rkOcctPoles,
				rkOcctWeights,
				rkOcctKnots,
				rkOcctMultiplicities,
				kDegree,
				false,
				kIsRational);
		}
		catch (Standard_Failure e)
		{
			throw std::exception(e.GetMessageString());
		}

		// Done separately, because SetPeriod() does additional steps to adjust the poles, weights, etc.
		if (kIsPeriodic)
		{
			try{
				pOcctBSplineCurve->SetPeriodic();
			}
			catch (Standard_Failure e)
			{
				throw std::exception(e.GetMessageString());
			}
		}

		return ByCurve(pOcctBSplineCurve);
	}

	Edge::Ptr Edge::ByCurve(Handle(Geom_Curve) pOcctCurve, const double rkParameter1, const double rkParameter2)
	{
		const double kOcctFirstParameter = pOcctCurve->FirstParameter();
		const double kOcctLastParameter = pOcctCurve->LastParameter();
		const double kOcctDeltaParameter = kOcctLastParameter - kOcctFirstParameter;

		// Compute the non-normalised parameters.
		const double kOcctParameter1 = kOcctFirstParameter + rkParameter1 * kOcctDeltaParameter;
		const double kOcctParameter2 = kOcctFirstParameter + rkParameter2 * kOcctDeltaParameter;

		BRepBuilderAPI_MakeEdge occtMakeEdge(pOcctCurve, kOcctParameter1, kOcctParameter2);
		if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
		{
			Throw(occtMakeEdge.Error());
		}

		ShapeFix_Shape occtEdgeFix(occtMakeEdge.Edge());
		Edge::Ptr pEdge = std::make_shared<Edge>(TopoDS::Edge(occtEdgeFix.Shape()));
		GlobalCluster::GetInstance().AddTopology(pEdge->GetOcctEdge());
		return pEdge;
	}

	Edge::Ptr Edge::ByStartVertexEndVertex(const std::shared_ptr<Vertex>& kpStartVertex, const std::shared_ptr<Vertex>& kpEndVertex)
	{
		if (kpStartVertex == nullptr || kpEndVertex == nullptr)
		{
			return nullptr;
		}

		BRepBuilderAPI_MakeEdge occtMakeEdge(
			kpStartVertex->GetOcctVertex(),
			kpEndVertex->GetOcctVertex());
		if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
		{
			Throw(occtMakeEdge.Error());
		}
		Edge::Ptr pEdge = std::make_shared<Edge>(occtMakeEdge.Edge());
		Vertex::Ptr startVertex = pEdge->StartVertex();
		Vertex::Ptr endVertex = pEdge->EndVertex();
		Edge::Ptr pCopyEdge = std::dynamic_pointer_cast<Edge>(pEdge->DeepCopy());
		AttributeManager::GetInstance().DeepCopyAttributes(startVertex->GetOcctVertex(), pCopyEdge->GetOcctEdge());
		AttributeManager::GetInstance().DeepCopyAttributes(endVertex->GetOcctVertex(), pCopyEdge->GetOcctEdge());
	
		GlobalCluster::GetInstance().AddTopology(pCopyEdge->GetOcctEdge());
		return pCopyEdge;
	}

	void Edge::SharedVertices(const Edge::Ptr& kpAnotherEdge, std::list<std::shared_ptr<Vertex>>& rSharedVertices) const
	{
		const TopoDS_Shape& rkOcctShape1 = GetOcctShape();
		TopTools_MapOfShape occtVertices1;
		DownwardNavigation(rkOcctShape1, TopAbs_VERTEX, occtVertices1);

		const TopoDS_Shape& rkOcctShape2 = kpAnotherEdge->GetOcctShape();
		TopTools_MapOfShape occtVertices2;
		DownwardNavigation(rkOcctShape2, TopAbs_VERTEX, occtVertices2);

		for (TopTools_MapIteratorOfMapOfShape occtVertexIterator1(occtVertices1);
			occtVertexIterator1.More();
			occtVertexIterator1.Next())
		{
			for (TopTools_MapIteratorOfMapOfShape occtVertexIterator2(occtVertices2);
				occtVertexIterator2.More();
				occtVertexIterator2.Next())
			{
				if (occtVertexIterator1.Value().IsSame(occtVertexIterator2.Value()))
				{
					Vertex::Ptr pVertex = std::make_shared<Vertex>(TopoDS::Vertex(occtVertexIterator1.Value()));
					rSharedVertices.push_back(pVertex);
				}
			}
		}
	}

	bool Edge::IsManifold() const
	{
		std::list<Wire::Ptr> wires;
		Wires(wires);

		// A manifold edge has <= 2 wires.
		if (wires.size() <= 2)
		{
			return true;
		}

		return false;
	}

	void Edge::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		rOcctGeometries.push_back(Curve());
	}

	TopoDS_Shape& Edge::GetOcctShape()
	{
		return GetOcctEdge();
	}

	const TopoDS_Shape& Edge::GetOcctShape() const
	{
		return GetOcctEdge();
	}

	TopoDS_Edge & Edge::GetOcctEdge()
	{
		assert(!m_occtEdge.IsNull() && "Edge::m_occtEdge is null.");
		if (m_occtEdge.IsNull())
		{
			throw std::exception("A null Edge is encountered.");
		}

		return m_occtEdge;
	}

	const TopoDS_Edge & Edge::GetOcctEdge() const
	{
		assert(!m_occtEdge.IsNull() && "Edge::m_occtEdge is null.");
		if (m_occtEdge.IsNull())
		{
			throw std::exception("A null Edge is encountered.");
		}

		return m_occtEdge;
	}

	void Edge::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		try {
			SetOcctEdge(TopoDS::Edge(rkOcctShape));
		}
		catch (Standard_Failure e)
		{
			throw std::exception(e.GetMessageString());
		}
	}

	void Edge::SetOcctEdge(const TopoDS_Edge & rkOcctEdge)
	{
		m_occtEdge = rkOcctEdge;
	}

	Handle(Geom_Curve) Edge::Curve() const
	{
		double u0 = 0.0, u1 = 0.0;
		return Curve(u0, u1);
	}

	Handle(Geom_Curve) Edge::Curve(double& rU0, double& rU1) const
	{
		return BRep_Tool::Curve(GetOcctEdge(), rU0, rU1);
	}

	double Edge::NormalizeParameter(const double kOcctMinParameter, const double kOcctMaxParameter, const double kNonNormalizedParameter)
	{
		double occtDParameter = kOcctMaxParameter - kOcctMinParameter;
		if (occtDParameter <= 0.0)
		{
			throw std::exception("Negative range");
		}

		return (kNonNormalizedParameter - kOcctMinParameter) / occtDParameter;
	}

	double Edge::NonNormalizeParameter(const double kOcctMinParameter, const double kOcctMaxParameter, const double kNormalizedParameter)
	{
		double occtDParameter = kOcctMaxParameter - kOcctMinParameter;
		return kOcctMinParameter + kNormalizedParameter * occtDParameter;
	}

	std::shared_ptr<Vertex> Edge::CenterOfMass() const
	{
		TopoDS_Vertex occtCenterOfMass = CenterOfMass(GetOcctEdge());
		return std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtCenterOfMass));
		/*GProp_GProps occtShapeProperties;
		BRepGProp::LinearProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));*/
	}

	TopoDS_Vertex Edge::CenterOfMass(const TopoDS_Edge & rkOcctEdge)
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::LinearProperties(rkOcctEdge, occtShapeProperties);
		return BRepBuilderAPI_MakeVertex(occtShapeProperties.CentreOfMass());
	}

	std::string Edge::GetTypeAsString() const
	{
		return std::string("Edge");
	}

	void Edge::Throw(const BRepBuilderAPI_EdgeError occtEdgeError)
	{
		switch (occtEdgeError)
		{
		case BRepBuilderAPI_PointProjectionFailed:
			throw std::exception("No parameters were given but the projection of the 3D points on the curve failed. This happens when the point distance to the curve is greater than the precision value.");
			
		case BRepBuilderAPI_ParameterOutOfRange:
			throw std::exception("The given parameters are not in the parametric range.");

		case BRepBuilderAPI_DifferentPointsOnClosedCurve:
			throw std::exception("The two vertices or points are the extremities of a closed curve but have different locations.");

		case BRepBuilderAPI_PointWithInfiniteParameter:
			throw std::exception("A finite coordinate point was associated with an infinite parameter.");

		case BRepBuilderAPI_DifferentsPointAndParameter:
			throw std::exception("The distance between the 3D point and the point evaluated on the curve with the parameter is greater than the precision.");

		default: //case BRepBuilderAPI_LineThroughIdenticPoints:
			throw std::exception("Two identical points were given to define a line (construction of an edge without curve).");
		}
	}

	Edge::Edge(const TopoDS_Edge& rkOcctEdge, const std::string& rkGuid)
		: Topology(1, rkOcctEdge, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtEdge(rkOcctEdge)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<EdgeFactory>());
	}

	Edge::~Edge()
	{

	}
}
