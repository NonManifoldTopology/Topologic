#include "Edge.h"
#include "Vertex.h"
#include "Wire.h"
#include "EdgeFactory.h"
#include "GlobalCluster.h"

#include <BRepGProp.hxx>
#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GProp_GProps.hxx>
#include <gp_Lin.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Edge::AdjacentEdges(std::list<Edge::Ptr>& rEdges) const
	{
		std::list<Vertex::Ptr> vertices;
		Vertices(vertices);

		// Find the constituent edges
		TopTools_MapOfShape occtEdges;
		for (const Vertex::Ptr& kpVertex : vertices)
		{
			// Find the edges
			std::list<Edge::Ptr> edges;
			kpVertex->Edges(edges);

			for (const Edge::Ptr& kpEdge : edges)
			{
				if (!IsSame(kpEdge) &&
					!occtEdges.Contains(kpEdge->GetOcctShape()))
				{
					occtEdges.Add(kpEdge->GetOcctShape());
					rEdges.push_back(kpEdge);
				}
			}
		}
	}

	void Edge::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		ShapeAnalysis_Edge occtShapeAnalysisEdge;
		
		TopoDS_Vertex occtVertex1 = occtShapeAnalysisEdge.FirstVertex(GetOcctEdge());
		rVertices.push_back(std::make_shared<Vertex>(occtVertex1));
		
		TopoDS_Vertex occtVertex2 = occtShapeAnalysisEdge.LastVertex(GetOcctEdge());
		rVertices.push_back(std::make_shared<Vertex>(occtVertex2));
	}

	void Edge::Wires(std::list<Wire::Ptr>& rWires) const
	{
		UpwardNavigation(rWires);
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

		Edge::Ptr pEdge = std::make_shared<Edge>(occtMakeEdge);
		GlobalCluster::GetInstance().AddTopology(pEdge->GetOcctEdge());
		return pEdge;
	}

	Edge::Ptr Edge::ByStartVertexEndVertex(const std::shared_ptr<Vertex>& kpStartVertex, const std::shared_ptr<Vertex>& kpEndVertex)
	{
		Vertex::Ptr startVertexCopy = std::dynamic_pointer_cast<Vertex>(kpStartVertex->Copy());
		Vertex::Ptr endVertexCopy = std::dynamic_pointer_cast<Vertex>(kpEndVertex->Copy());
		BRepBuilderAPI_MakeEdge occtMakeEdge(
			startVertexCopy->GetOcctVertex(),
			endVertexCopy->GetOcctVertex());
		if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
		{
			Throw(occtMakeEdge.Error());
		}
		Edge::Ptr pEdge = std::make_shared<Edge>(occtMakeEdge.Edge());
		GlobalCluster::GetInstance().AddTopology(pEdge->GetOcctEdge());
		return pEdge;
	}

	Vertex::Ptr Edge::SharedVertex(const Edge::Ptr& kpAnotherEdge) const
	{
		TopoDS_Vertex occtSharedVertex;
		bool hasSharedVertex = TopExp::CommonVertex(m_occtEdge, kpAnotherEdge->GetOcctEdge(), occtSharedVertex);
		if (!hasSharedVertex)
		{
			return nullptr;
		}

		Vertex::Ptr pVertex = std::make_shared<Vertex>(occtSharedVertex);

		// Find which of the vertices is this vertex, then copy the attributes. 
		return pVertex;
	}

	bool Edge::IsManifold() const
	{
		throw std::exception("Not implemented yet");
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
			throw std::exception("Edge::m_occtEdge is null.");
		}

		return m_occtEdge;
	}

	const TopoDS_Edge & Edge::GetOcctEdge() const
	{
		assert(!m_occtEdge.IsNull() && "Edge::m_occtEdge is null.");
		if (m_occtEdge.IsNull())
		{
			throw std::exception("Edge::m_occtEdge is null.");
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
		GProp_GProps occtShapeProperties;
		BRepGProp::LinearProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
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
