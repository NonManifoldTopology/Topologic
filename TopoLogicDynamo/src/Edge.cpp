#include "Edge.h"
#include <Vertex.h>
#include <Wire.h>

#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <GeomConvert.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Line.hxx>
#include <Geom_OffsetCurve.hxx>
#include <TopoDS.hxx>

#include <assert.h>

namespace Topologic
{
	Edge^ Edge::ByCircle(
		Vertex^ centerPoint, double radius,
		double xAxisX, double xAxisY, double xAxisZ,
		double normalX, double normalY, double normalZ)
	{
		TopologicCore::Vertex::Ptr pCoreCentrePoint = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(centerPoint->GetCoreTopologicalQuery());
		TopologicCore::Edge::Ptr pCoreCircleEdge = TopologicCore::Edge::ByCircle(pCoreCentrePoint, radius, xAxisX, xAxisY, xAxisZ, normalX, normalY, normalZ);
		return gcnew Edge(pCoreCircleEdge);
	}

	List<Edge^>^ Edge::AdjacentEdges_(Topology ^ hostTopology)
	{
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreHostTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(hostTopology->GetCoreTopologicalQuery());
		std::list<TopologicCore::Edge::Ptr> pAdjacentCoreEdges;
		pCoreEdge->AdjacentEdges(pCoreHostTopology, pAdjacentCoreEdges);

		List<Edge^>^ pAdjacentEdges = gcnew List<Edge^>();

		for (std::list<TopologicCore::Edge::Ptr>::const_iterator kAdjacentEdgeIterator = pAdjacentCoreEdges.begin();
			kAdjacentEdgeIterator != pAdjacentCoreEdges.end();
			kAdjacentEdgeIterator++)
		{
			const TopologicCore::Edge::Ptr& kpCoreEdge = *kAdjacentEdgeIterator;
			Edge^ pEdge = gcnew Edge(kpCoreEdge);
			pAdjacentEdges->Add(pEdge);
		}

		return pAdjacentEdges;
	}

	List<Vertex^>^ Edge::Vertices::get()
	{
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		pCoreEdge->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<TopologicCore::Vertex::Ptr>::iterator coreVertexIterator = coreVertices.begin();
			coreVertexIterator != coreVertices.end();
			coreVertexIterator++)
		{
			const TopologicCore::Vertex::Ptr& kpCoreVertex = *coreVertexIterator;
			Vertex^ pVertex = gcnew Vertex(kpCoreVertex);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	List<Wire^>^ Edge::Wires_(Topology^ hostTopology)
	{
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		std::shared_ptr<TopologicCore::Topology> pCoreHostTopology = TopologicCore::Topology::Downcast<TopologicCore::Topology>(hostTopology->GetCoreTopologicalQuery());
		std::list<TopologicCore::Wire::Ptr> coreWires;
		pCoreEdge->Wires(pCoreHostTopology, coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for(std::list<TopologicCore::Wire::Ptr>::iterator coreWireIterator = coreWires.begin();
			coreWireIterator != coreWires.end();
			coreWireIterator++)
		{
			const TopologicCore::Wire::Ptr& kpCoreWire = *coreWireIterator;
			Wire^ pWire = gcnew Wire(kpCoreWire);
			pWires->Add(pWire);
		}
		
		return pWires;
	}

	Edge^ Edge::ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve)
	{
		if (curve->GetType() == Autodesk::DesignScript::Geometry::Arc::typeid)
		{
			//throw gcnew System::NotImplementedException("Feature not yet implemented.");
			return ByCurve(curve->ToNurbsCurve());
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Circle::typeid)
		{
			return ByCurve(safe_cast<Autodesk::DesignScript::Geometry::Circle^>(curve));
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Ellipse::typeid)
		{
			//throw gcnew System::NotImplementedException("Feature not yet implemented.");
			return ByCurve(curve->ToNurbsCurve());
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::EllipseArc::typeid)
		{
			//throw gcnew System::NotImplementedException("Feature not yet implemented.");
			return ByCurve(curve->ToNurbsCurve());
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Helix::typeid)
		{
			//throw gcnew System::NotImplementedException("Feature not yet implemented.");
			return ByCurve(curve->ToNurbsCurve());
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Line::typeid)
		{
			return ByCurve(safe_cast<Autodesk::DesignScript::Geometry::Line^>(curve));
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::NurbsCurve::typeid)
		{
			return ByCurve(safe_cast<Autodesk::DesignScript::Geometry::NurbsCurve^>(curve));
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::PolyCurve::typeid)
		{
			throw gcnew System::NotImplementedException("Cannot create an Edge from a PolyCurve. Create a wire instead.");
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Curve::typeid) // a generic curve
		{
			return ByCurve(curve->ToNurbsCurve());
		}
		else
		{
			throw gcnew ArgumentException("The argument is not a valid Dynamo curve.");
		}
	}

	Edge^ Edge::ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices)
	{
		std::list<TopologicCore::Vertex::Ptr> pCoreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			pCoreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
		}
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Edge::ByVertices(pCoreVertices);
		return gcnew Edge(pCoreEdge);
	}

	Edge ^ Edge::ByStartVertexEndVertex(Vertex ^ startVertex, Vertex ^ endVertex)
	{
		TopologicCore::Vertex::Ptr pCoreStartVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(startVertex->GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreEndVertex = TopologicCore::Topology::Downcast<TopologicCore::Vertex>(endVertex->GetCoreTopologicalQuery());
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Edge::ByStartVertexEndVertex(pCoreStartVertex, pCoreEndVertex);
		return gcnew Edge(pCoreEdge);
	}

	Vertex^ Edge::SharedVertex(Edge^ edge)
	{
		TopologicCore::Edge::Ptr pCoreEdge1 = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		TopologicCore::Edge::Ptr pCoreEdge2 = TopologicCore::Topology::Downcast<TopologicCore::Edge>(edge->GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreVertex = pCoreEdge1->SharedVertex(pCoreEdge2);

		return gcnew Vertex(pCoreVertex);
	}

	Autodesk::DesignScript::Geometry::Curve^ Edge::Curve()
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = BRep_Tool::Curve(TopoDS::Edge(TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(GetCoreTopologicalQuery())->GetOcctShape()), u0, u1);

		return Curve(pOcctCurve, u0, u1);
	}

	double Edge::ParameterAtVertex(Vertex^ vertex)
	{
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		return pCoreEdge->ParameterAtPoint(TopologicCore::Topology::Downcast<TopologicCore::Vertex>(vertex->GetCoreTopologicalQuery()));
	}

	Vertex^ Edge::VertexAtParameter(double parameter)
	{
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		TopologicCore::Vertex::Ptr pCoreVertex = pCoreEdge->PointAtParameter(parameter);
		return gcnew Vertex(pCoreVertex);
	}

	Object^ Edge::Geometry::get()
	{
		return Curve();
	}

	Autodesk::DesignScript::Geometry::Curve^ DynamoCurveFromBSplineCurve(Handle(Geom_BSplineCurve) pOcctBsplineCurve, const double u0, const double u1)
	{
		bool isPeriodic = pOcctBsplineCurve->IsPeriodic();
		if (isPeriodic)
			pOcctBsplineCurve->SetNotPeriodic();

		// Transfer the control points
		const TColgp_Array1OfPnt& rkOcctControlPoints = pOcctBsplineCurve->Poles();
		array<Autodesk::DesignScript::Geometry::Point^>^ pDynamoControlPoints = gcnew array<Autodesk::DesignScript::Geometry::Point^>(rkOcctControlPoints.Length());
		for (int i = rkOcctControlPoints.Lower(); i <= rkOcctControlPoints.Upper(); i++)
		{
			const gp_Pnt& rkControlPoint = rkOcctControlPoints.Value(i);
			pDynamoControlPoints[i - rkOcctControlPoints.Lower()] = Autodesk::DesignScript::Geometry::Point::ByCoordinates(rkControlPoint.X(), rkControlPoint.Y(), rkControlPoint.Z());
		}

		// Transfer the control points' weights
		const TColStd_Array1OfReal* pkOcctWeights = pOcctBsplineCurve->Weights();
		array<double>^ pWeights = nullptr;
		// if null, every weight is assumed to be one, i.e. a uniform curve
		if (pkOcctWeights == nullptr)
		{
			pWeights = gcnew array<double>(rkOcctControlPoints.Length());
			for (int i = 0; i < pWeights->Length; i++)
			{
				pWeights[i] = 1.0;
			}
		}
		else
		{
			pWeights = gcnew array<double>(rkOcctControlPoints.Length());
			assert(pkOcctWeights->Length() == rkOcctControlPoints.Length() && "Weights and control points have different lengths.");
			for (int i = pkOcctWeights->Lower(); i <= pkOcctWeights->Upper(); i++)
			{
				pWeights[i - pkOcctWeights->Lower()] = pkOcctWeights->Value(i);
			}
		}

		// Transfer the knots. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
		const TColStd_Array1OfReal& krOcctKnots = pOcctBsplineCurve->Knots();
		List<double>^ pKnots = gcnew List<double>();
		for (int i = krOcctKnots.Lower(); i <= krOcctKnots.Upper(); i++)
		{
			int multiplicity = pOcctBsplineCurve->Multiplicity(i);
			for (int j = 0; j < multiplicity; j++)
			{
				double occtKnot = krOcctKnots.Value(i);
				pKnots->Add(occtKnot);
			}
		}

		// OCCT has arbitrary parameters. Dynamo's parameter ranges between 0 and 1.
		// Order: First - Start - End - Last
		double occtFirstParameter = pOcctBsplineCurve->FirstParameter();
		double occtLastParameter = pOcctBsplineCurve->LastParameter();
		double occtDeltaParameter = occtLastParameter - occtFirstParameter;
		if (occtDeltaParameter < Precision::Confusion())
			occtDeltaParameter = Precision::Confusion();
		double dynamoStartParameter = (u0 - occtFirstParameter) / occtDeltaParameter;
		double dynamoEndParameter = (u1 - occtFirstParameter) / occtDeltaParameter;

		int degree = pOcctBsplineCurve->Degree();
		Autodesk::DesignScript::Geometry::Curve^ pDynamoNurbsCurve =
			Autodesk::DesignScript::Geometry::NurbsCurve::ByControlPointsWeightsKnots(pDynamoControlPoints, pWeights, pKnots->ToArray(), degree)
			->TrimByParameter(dynamoStartParameter, dynamoEndParameter);

		for each(Autodesk::DesignScript::Geometry::Point^ pDynamoControlPoint in pDynamoControlPoints)
		{
			delete pDynamoControlPoint;
		}

		return pDynamoNurbsCurve;
	}

	Autodesk::DesignScript::Geometry::Curve^ Edge::Curve(Handle(Geom_Curve) pOcctCurve, const double u0, const double u1)
	{
		Handle(Geom_BezierCurve) pOcctBezierCurve = Handle_Geom_BezierCurve::DownCast(pOcctCurve);
		if (!pOcctBezierCurve.IsNull())
		{
			Handle(Geom_BSplineCurve) pOcctBsplineCurve = GeomConvert::CurveToBSplineCurve(pOcctBezierCurve);
			return DynamoCurveFromBSplineCurve(pOcctBsplineCurve, u0, u1);
		}

		Handle(Geom_BSplineCurve) pOcctBsplineCurve = Handle_Geom_BSplineCurve::DownCast(pOcctCurve);
		if (!pOcctBsplineCurve.IsNull())
		{
			return DynamoCurveFromBSplineCurve(pOcctBsplineCurve, u0, u1);
		}

		Handle(Geom_TrimmedCurve) pOcctTrimmedCurve = Handle_Geom_TrimmedCurve::DownCast(pOcctCurve);
		if (!pOcctTrimmedCurve.IsNull())
		{
			return Curve(pOcctTrimmedCurve->BasisCurve(), pOcctTrimmedCurve->FirstParameter(), pOcctTrimmedCurve->LastParameter());
		}

		Handle(Geom_Circle) pOcctCircle = Handle_Geom_Circle::DownCast(pOcctCurve);
		if (!pOcctCircle.IsNull())
		{
			const gp_Ax2& rkOcctPosition = pOcctCircle->Position();
			Autodesk::DesignScript::Geometry::Point^ pDynamoCenterPoint = Autodesk::DesignScript::Geometry::Point::ByCoordinates(rkOcctPosition.Location().X(), rkOcctPosition.Location().Y(), rkOcctPosition.Location().Z());
			Autodesk::DesignScript::Geometry::Vector^ pDynamoNormal = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(rkOcctPosition.Direction().X(), rkOcctPosition.Direction().Y(), rkOcctPosition.Direction().Z());

			Autodesk::DesignScript::Geometry::Circle^ pDynamoCircle = Autodesk::DesignScript::Geometry::Circle::ByCenterPointRadiusNormal(
				pDynamoCenterPoint,
				pOcctCircle->Radius(),
				pDynamoNormal
			);

			delete pDynamoCenterPoint;
			delete pDynamoNormal;

			return pDynamoCircle;
		}

		Handle(Geom_Ellipse) pOcctEllipse = Handle_Geom_Ellipse::DownCast(pOcctCurve);
		if (!pOcctEllipse.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_Hyperbola) pOcctHyperbola = Handle_Geom_Hyperbola::DownCast(pOcctCurve);
		if (!pOcctHyperbola.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_Parabola) pOcctParabola = Handle_Geom_Parabola::DownCast(pOcctCurve);
		if (!pOcctParabola.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_OffsetCurve) pOcctOffsetCurve = Handle_Geom_OffsetCurve::DownCast(pOcctCurve);
		if (!pOcctOffsetCurve.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		// The default is a line:
		// Handle(Geom_Line) pOcctLine = Handle_Geom_Line::DownCast(pOcctCurve);

		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Topology::Downcast<TopologicCore::Edge>(GetCoreTopologicalQuery());
		List<Vertex^>^ pVertices = Vertices;

		bool onlyTwoVertices = true;
		if (pVertices->Count != 2)
		{
			onlyTwoVertices = false;
		}

		Autodesk::DesignScript::Geometry::Point^ pDynamoPoint1 = safe_cast<Autodesk::DesignScript::Geometry::Point^>(pVertices[0]->Geometry);
		Autodesk::DesignScript::Geometry::Point^ pDynamoPoint2 = safe_cast<Autodesk::DesignScript::Geometry::Point^>(pVertices[1]->Geometry);

		Autodesk::DesignScript::Geometry::Line^ pDynamoLine =
			Autodesk::DesignScript::Geometry::Line::ByStartPointEndPoint(pDynamoPoint1, pDynamoPoint2);

		delete pDynamoPoint1;
		delete pDynamoPoint2;

		if (!onlyTwoVertices)
		{
			throw gcnew Exception("The edge does not have exactly 2 vertices");
		}
		return pDynamoLine;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Edge::GetCoreTopologicalQuery()
	{
		assert(m_pCoreEdge != nullptr && "Edge::m_pCoreEdge is null.");
		if (m_pCoreEdge == nullptr)
		{
			throw gcnew Exception("Edge::m_pCoreEdge is null.");
		}

		return *m_pCoreEdge;
	}

	Edge::Edge(const TopologicCore::Edge::Ptr& kpCoreEdge)
		: Topology()
		, m_pCoreEdge(new TopologicCore::Edge::Ptr(kpCoreEdge))
	{

	}

	Edge::~Edge()
	{
		//delete m_pCoreEdge;
	}

	Edge^ Edge::ByCurve(Autodesk::DesignScript::Geometry::NurbsCurve^ pDynamoNurbsCurve)
	{
		// Transfer the poles/control points
		array<Autodesk::DesignScript::Geometry::Point^>^ pDynamoControlPoints = pDynamoNurbsCurve->ControlPoints();
		TColgp_Array1OfPnt occtPoles(0, pDynamoControlPoints->Length - 1);
		for (int i = occtPoles.Lower(); i <= occtPoles.Upper(); i++)
		{
			occtPoles.SetValue(i, gp_Pnt(pDynamoControlPoints[i]->X, pDynamoControlPoints[i]->Y, pDynamoControlPoints[i]->Z));
		}

		// Transfer the weights
		array<double>^ pDynamoWeights = pDynamoNurbsCurve->Weights();
		TColStd_Array1OfReal occtWeights(0, pDynamoWeights->Length - 1);
		for (int i = occtWeights.Lower(); i <= occtWeights.Upper(); i++)
		{
			double weight = pDynamoWeights[i];
			occtWeights.SetValue(i, weight);
		}

		// Transfer the knots and multiplicities. Note the format difference. OCCT has a separate multiplicity list, while Dynamo simply repeats the knots.
		array<double>^ pDynamoKnots = pDynamoNurbsCurve->Knots();
		List<double>^ pKnots = gcnew List<double>();
		List<int>^ pMultiplicities = gcnew List<int>();
		double previousKnot = pDynamoKnots[0] - 1.0;
		int multiplicity = 0;
		for each(double knot in pDynamoKnots)
		{
			if (knot > previousKnot)
			{
				if (previousKnot > pDynamoKnots[0] - 1.0)
					pMultiplicities->Add(multiplicity);
				pKnots->Add(knot);
				multiplicity = 1;
			}
			else
			{
				multiplicity++;
			}
			previousKnot = knot;
		}
		pMultiplicities->Add(multiplicity);

		TColStd_Array1OfReal occtKnots(0, pKnots->Count - 1);
		for (int i = occtKnots.Lower(); i <= occtKnots.Upper(); i++)
		{
			occtKnots.SetValue(i, pKnots[i]);
		}

		TColStd_Array1OfInteger occtMultiplicities(0, pMultiplicities->Count - 1);
		for (int i = occtMultiplicities.Lower(); i <= occtMultiplicities.Upper(); i++)
		{
			occtMultiplicities.SetValue(i, pMultiplicities[i]);
		}

		bool isRational = pDynamoNurbsCurve->IsRational;
		bool isPeriodic = pDynamoNurbsCurve->IsPeriodic;
		int degree = pDynamoNurbsCurve->Degree;

		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Edge::ByCurve(
			occtPoles,
			occtWeights,
			occtKnots,
			occtMultiplicities,
			degree,
			isPeriodic,
			isRational
			);

		for each(Autodesk::DesignScript::Geometry::Point^ pDynamoControlPoint in pDynamoControlPoints)
		{
			delete pDynamoControlPoint;
		}

		return gcnew Edge(pCoreEdge);
	}

	Edge^ Edge::ByCurve(Autodesk::DesignScript::Geometry::Circle^ pDynamoCircle)
	{
		Autodesk::DesignScript::Geometry::Point^ pDynamoCenterPoint = pDynamoCircle->CenterPoint;
		double radius = pDynamoCircle->Radius;
		Autodesk::DesignScript::Geometry::Vector^ pDynamoNormal = pDynamoCircle->Normal;
		pDynamoNormal = pDynamoNormal->Normalized();
		Autodesk::DesignScript::Geometry::CoordinateSystem^ pDynamoCoordinateSystem = pDynamoCircle->ContextCoordinateSystem;
		Autodesk::DesignScript::Geometry::Vector^ pDynamoXAxis = pDynamoCoordinateSystem->XAxis;
		pDynamoXAxis = pDynamoXAxis->Normalized();

		Handle(Geom_Circle) pOcctCircle = new Geom_Circle(
			gp_Ax2(
				gp_Pnt(pDynamoCenterPoint->X, pDynamoCenterPoint->Y, pDynamoCenterPoint->Z),
				gp_Dir(pDynamoNormal->X, pDynamoNormal->Y, pDynamoNormal->Z),
				gp_Dir(pDynamoXAxis->X, pDynamoXAxis->Y, pDynamoXAxis->Z)
				),
			radius
			);

		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Edge::ByCurve(pOcctCircle);

		delete pDynamoCenterPoint;
		delete pDynamoNormal;
		delete pDynamoCoordinateSystem;
		delete pDynamoXAxis;

		return gcnew Edge(pCoreEdge);
	}

	Edge^ Edge::ByCurve(Autodesk::DesignScript::Geometry::Line^ pDynamoLine)
	{
		std::list<TopologicCore::Vertex::Ptr> coreVertices;
		coreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>((gcnew Vertex(pDynamoLine->StartPoint))->GetCoreTopologicalQuery()));
		coreVertices.push_back(TopologicCore::Topology::Downcast<TopologicCore::Vertex>((gcnew Vertex(pDynamoLine->EndPoint))->GetCoreTopologicalQuery()));
		TopologicCore::Edge::Ptr pCoreEdge = TopologicCore::Edge::ByVertices(coreVertices);
		return gcnew Edge(pCoreEdge);
	}
}