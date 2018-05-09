#include <Edge.h>
#include <Vertex.h>
#include <Wire.h>
#include <LabelManager.h>
#include <OcctCounterAttribute.h>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <GeomLib_Tool.hxx>
#include <gp_Lin.hxx>
#include <Precision.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <TopExp.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void Edge::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		ShapeAnalysis_Edge occtShapeAnalysisEdge;
		
		TopoDS_Vertex occtVertex1 = occtShapeAnalysisEdge.FirstVertex(GetOcctEdge());
		// Find the vertex's label
		TDF_Label occtVertex1Label;
		LabelManager::GetInstance().FindChildLabelByShape(occtVertex1, occtVertex1Label);
		rVertices.push_back(std::make_shared<Vertex>(occtVertex1, occtVertex1Label));
		
		TopoDS_Vertex occtVertex2 = occtShapeAnalysisEdge.LastVertex(GetOcctEdge());
		// Find the vertex's label
		TDF_Label occtVertex2Label;
		LabelManager::GetInstance().FindChildLabelByShape(occtVertex2, occtVertex2Label);

		rVertices.push_back(std::make_shared<Vertex>(occtVertex2, occtVertex2Label));
	}

	void Edge::Wires(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Wire>>& rWires) const
	{
		UpwardNavigation(kpParentTopology, rWires);
	}

	std::shared_ptr<Edge> Edge::ByCurve(
		const TColgp_Array1OfPnt &rkOcctPoles,
		const TColStd_Array1OfReal &rkOcctWeights,
		const TColStd_Array1OfReal &rkOcctKnots,
		const TColStd_Array1OfInteger &rkOcctMultiplicities,
		const int kDegree,
		const bool kIsPeriodic,
		const bool kIsRational)
	{
		BRepBuilderAPI_MakeEdge occtMakeEdge;
		Handle(Geom_BSplineCurve) pOcctBSplineCurve = new Geom_BSplineCurve(
			rkOcctPoles,
			rkOcctWeights,
			rkOcctKnots,
			rkOcctMultiplicities,
			kDegree,
			false,
			kIsRational);

		// Done separately, because SetPeriod() does additional steps to adjust the poles, weights, etc.
		if (kIsPeriodic)
		{
			pOcctBSplineCurve->SetPeriodic();
		}

		return ByCurve(pOcctBSplineCurve);
	}

	std::shared_ptr<Edge> Edge::ByCurve(Handle(Geom_Curve) pOcctCurve, const double rkParameter1, const double rkParameter2)
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

		std::shared_ptr<Edge> pEdge = std::make_shared<Edge>(occtMakeEdge);
		LabelManager::GetInstance().AddGeneratedMembersToLabel(pEdge->GetOcctLabel());
		return pEdge;
	}

	std::shared_ptr<Edge> Edge::ByVertices(const std::list<std::shared_ptr<Vertex>>& rkVertices)
	{
		// Cases:
		// - 0 or 1 vertex --> invalid
		// - 2 --> line
		// - 3 --> BSpline

		int numberOfVertices = (int) rkVertices.size();
		std::shared_ptr<Edge> pEdge = nullptr;
		if (numberOfVertices < 2)
		{
			throw std::exception("Too few vertices to create an edge.");
		}
		else if (numberOfVertices == 2) // a line
		{
			const std::shared_ptr<Vertex>& rkVertex1 = *(rkVertices.begin());
			const std::shared_ptr<Vertex>& rkVertex2 = *(++rkVertices.begin());
			BRepBuilderAPI_MakeEdge occtMakeEdge(
				rkVertex1->GetOcctVertex(),
				rkVertex2->GetOcctVertex());
			pEdge = std::make_shared<Edge>(occtMakeEdge.Edge());

			const std::shared_ptr<Topology>& rkBaseVertex1 = TopologicalQuery::Upcast<Topology>(rkVertex1);
			const std::shared_ptr<Topology>& rkBaseVertex2 = TopologicalQuery::Upcast<Topology>(rkVertex2);
			std::list<std::pair<std::shared_ptr<Topology>, std::shared_ptr<Topology>>> topologyPairs;
			topologyPairs.push_back(std::make_pair(rkBaseVertex1, rkBaseVertex1));
			topologyPairs.push_back(std::make_pair(rkBaseVertex2, rkBaseVertex2));

			// Add the vertices to the edge's label. Must do this manually because of the Modified()'s nature to map 
			// old to new sub-shapes.
			LabelManager::GetInstance().AddModifiedMembers(
				pEdge->GetOcctLabel(), 
				topologyPairs);

		}else
		{
			// else more than 2 vertices
			Handle(TColgp_HArray1OfPnt) pOcctPoints = new TColgp_HArray1OfPnt(1, numberOfVertices);
			int i = 1;
			for(const std::shared_ptr<Vertex>& kpVertex : rkVertices)
			{
				pOcctPoints->SetValue(i, kpVertex->Point()->Pnt());
				++i;
			}

			try {
				GeomAPI_Interpolate occtInterpolate(pOcctPoints, false, Precision::Confusion());
				occtInterpolate.Perform();
				if (!occtInterpolate.IsDone())
				{
					throw std::exception("Line interpolation error in Edge::ByVertices()");
				}
				Handle(Geom_Curve) pOcctCurveOnTargetSurface = occtInterpolate.Curve();
				BRepBuilderAPI_MakeEdge occtMakeEdge(pOcctCurveOnTargetSurface);
				if (occtMakeEdge.Error() != BRepBuilderAPI_EdgeDone)
				{
					Throw(occtMakeEdge.Error());
				}

				pEdge = std::make_shared<Edge>(occtMakeEdge);
				LabelManager::GetInstance().AddGeneratedMembersToLabel(pEdge->GetOcctLabel());
			}
			catch (Standard_ConstructionError e)
			{
				throw std::exception(e.GetMessageString());
			}
			catch (Standard_OutOfRange e)
			{
				throw std::exception(e.GetMessageString());
			}
		}

		return pEdge;
	}

	std::shared_ptr<Vertex> Edge::SharedVertex(const std::shared_ptr<Edge>& kpAnotherEdge) const
	{
		TopoDS_Vertex occtSharedVertex;
		bool hasSharedVertex = TopExp::CommonVertex(m_occtEdge, kpAnotherEdge->GetOcctEdge(), occtSharedVertex);
		if (!hasSharedVertex)
		{
			throw std::exception("The two edges have no shared vertex");
		}

		std::shared_ptr<Vertex> pVertex = std::make_shared<Vertex>(occtSharedVertex);

		// Find which of the vertices is this vertex, then copy the attributes. 
		return pVertex;
	}

	double Edge::ParameterAtPoint(const std::shared_ptr<Vertex>& kpVertex) const
	{
		Handle(Geom_Curve) pOcctCurve = Curve();
		Handle(Geom_Point) pOcctPoint = kpVertex->Point();

		double occtParameter = 0.0;
		bool isOnCurve = GeomLib_Tool::Parameter(pOcctCurve, pOcctPoint->Pnt(), Precision::Confusion(), occtParameter);
		if (!isOnCurve)
		{
			throw std::exception("Point not on curve");
		}

		// Parameter may be non-normalized, so normalize it
		return NormalizeParameter(pOcctCurve, occtParameter);
	}

	std::shared_ptr<Vertex> Edge::PointAtParameter(const double kParameter) const
	{
		Handle(Geom_Curve) pOcctCurve = Curve();

		// Parameter is normalized, so non-normalize it
		double occtParameter = NonNormalizeParameter(pOcctCurve, kParameter);
		gp_Pnt occtPoint = pOcctCurve->Value(occtParameter);
		
		return Vertex::ByPoint(new Geom_CartesianPoint(occtPoint));
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

	Handle(Geom_Curve) Edge::Curve() const
	{
		// TODO: do these parameters need to be stored?
		double u0 = 0.0, u1 = 0.0;
		return BRep_Tool::Curve(GetOcctEdge(), u0, u1);
	}

	double Edge::NormalizeParameter(Handle(Geom_Curve) pOcctCurve, const double kNonNormalizedParameter)
	{
		double occtMinParameter = pOcctCurve->FirstParameter();
		double occtMaxParameter = pOcctCurve->LastParameter();
		double occtDParameter = occtMaxParameter - occtMinParameter;
		if (occtDParameter <= 0.0)
		{
			throw std::exception("Negative range");
		}

		return (kNonNormalizedParameter - occtMinParameter) / occtDParameter;
	}

	double Edge::NonNormalizeParameter(Handle(Geom_Curve) pOcctCurve, const double kNormalizedParameter)
	{
		double occtMinParameter = pOcctCurve->FirstParameter();
		double occtMaxParameter = pOcctCurve->LastParameter();
		double occtDParameter = occtMaxParameter - occtMinParameter;

		return occtMinParameter + kNormalizedParameter * occtDParameter;
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

	Edge::Edge(const TopoDS_Edge& rkOcctEdge, const TDF_Label& rkOcctLabel)
		: Topology(1)
		, m_occtEdge(rkOcctEdge)
	{
		//GlobalCluster::GetInstance().Add(this);

		// Needs to be done in the subclass, not in Topology, as the OCCT shape is not yet defined there.
		SetOcctLabel(rkOcctLabel);
		OcctCounterAttribute::IncreaseCounter(GetOcctLabel());
	}

	Edge::~Edge()
	{
		//GlobalCluster::GetInstance().Remove(this);
		DecreaseCounter();
	}
}
