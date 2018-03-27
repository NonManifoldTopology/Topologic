#include <Edge.h>
#include <GlobalCluster.h>
#include <Vertex.h>
#include <Wire.h>

#include <BRepBuilderAPI_MakeEdge.hxx>
#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_Point.hxx>
#include <GeomAPI_Interpolate.hxx>
#include <gp_Lin.hxx>
#include <Precision.hxx>
#include <ShapeAnalysis_Edge.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void Edge::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		ShapeAnalysis_Edge shapeAnalysisEdge;
		rVertices.push_back(std::make_shared<Vertex>(shapeAnalysisEdge.FirstVertex(TopoDS::Edge(*GetOcctShape()))));
		rVertices.push_back(std::make_shared<Vertex>(shapeAnalysisEdge.LastVertex(TopoDS::Edge(*GetOcctShape()))));
	}

	void Edge::Wires(std::list<std::shared_ptr<Wire>>& rWires) const
	{
		UpwardNavigation(rWires);
	}

	std::shared_ptr<Edge> Edge::ByCurve(
		const TColgp_Array1OfPnt &rkOcctPoles,
		const TColStd_Array1OfReal &rkOcctWeights,
		const TColStd_Array1OfReal &rkOcctKnots,
		const TColStd_Array1OfInteger &rkOcctMultiplicities,
		const Standard_Integer kDegree,
		const Standard_Boolean kIsPeriodic,
		const Standard_Boolean kIsRational)
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

		return std::make_shared<Edge>(BRepBuilderAPI_MakeEdge(pOcctCurve, kOcctParameter1, kOcctParameter2));
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
			BRepBuilderAPI_MakeEdge occtMakeEdge(TopoDS::Vertex(*rkVertex1->GetOcctShape()),
				TopoDS::Vertex(*rkVertex2->GetOcctShape()));
			pEdge = std::make_shared<Edge>(occtMakeEdge.Edge());
		}else
		{
			// else more than 2 vertices
			Handle(TColgp_HArray1OfPnt) pOcctPoints = new TColgp_HArray1OfPnt(1, numberOfVertices);
			int i = 1;
			for(std::list<std::shared_ptr<Vertex>>::const_iterator kVertexIterator = rkVertices.begin();
				kVertexIterator != rkVertices.end();
				kVertexIterator++)
			{
				const std::shared_ptr<Vertex>& pVertex = *kVertexIterator;
				pOcctPoints->SetValue(i, pVertex->Point()->Pnt());
				++i;
			}
			GeomAPI_Interpolate occtInterpolate(pOcctPoints, false, Precision::Confusion());
			occtInterpolate.Perform();
			Handle(Geom_Curve) pOcctCurveOnTargetSurface = occtInterpolate.Curve();

			pEdge = std::make_shared<Edge>(BRepBuilderAPI_MakeEdge(pOcctCurveOnTargetSurface));
		}

		// Register the ingredients
		for (std::list<std::shared_ptr<Vertex>>::const_iterator kVertexIterator = rkVertices.begin();
			kVertexIterator != rkVertices.end();
			kVertexIterator++)
		{
			const std::shared_ptr<Vertex>& kpVertex = *kVertexIterator;
			kpVertex->AddIngredientTo(pEdge);
		}

		return pEdge;
	}

	std::shared_ptr<Vertex> Edge::SharedVertex(Edge const * const kpkAnotherEdge) const
	{
		TopoDS_Vertex occtSharedVertex;
		bool result = TopExp::CommonVertex(*m_pOcctEdge, TopoDS::Edge(*kpkAnotherEdge->GetOcctShape()), occtSharedVertex);

		return std::make_shared<Vertex>(occtSharedVertex);
	}

	void Edge::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		rOcctGeometries.push_back(Curve());
	}

	std::shared_ptr<TopoDS_Shape> Edge::GetOcctShape() const
	{
		assert(m_pOcctEdge != nullptr && "Edge::m_pOcctEdge is null.");
		if (m_pOcctEdge == nullptr)
		{
			throw std::exception("Edge::m_pOcctEdge is null.");
		}

		return m_pOcctEdge;
	}

	Handle(Geom_Curve) Edge::Curve() const
	{
		// TODO: do these parameters need to be stored?
		double u0 = 0.0, u1 = 0.0;
		return BRep_Tool::Curve(TopoDS::Edge(*GetOcctShape()), u0, u1);
	}

	Edge::Edge(const TopoDS_Edge& rkOcctEdge)
		: Topology(1)
		, m_pOcctEdge(std::make_shared<TopoDS_Edge>(rkOcctEdge))
	{
		GlobalCluster::GetInstance().Add(this);
	}

	Edge::~Edge()
	{
		GlobalCluster::GetInstance().Remove(this);
	}
}
