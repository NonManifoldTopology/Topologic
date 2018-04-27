#include "Edge.h"
#include <Vertex.h>
#include <Wire.h>

#include <BRep_Tool.hxx>
#include <Geom_BSplineCurve.hxx>
#include <Geom_BSplineSurface.hxx>
#include <Geom_BezierCurve.hxx>
#include <Geom_TrimmedCurve.hxx>
#include <Geom_Circle.hxx>
#include <Geom_Ellipse.hxx>
#include <Geom_Hyperbola.hxx>
#include <Geom_Parabola.hxx>
#include <Geom_Line.hxx>
#include <Geom_OffsetCurve.hxx>
#include <TopoDS.hxx>

#include <assert.h>

namespace TopoLogic
{
	List<Vertex^>^ Edge::Vertices()
	{
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Vertex>> coreVertices;
		pCoreEdge->Vertices(coreVertices);

		List<Vertex^>^ pVertices = gcnew List<Vertex^>();
		for (std::list<std::shared_ptr<TopoLogicCore::Vertex>>::iterator coreVertexIterator = coreVertices.begin();
			coreVertexIterator != coreVertices.end();
			coreVertexIterator++)
		{
			const std::shared_ptr<TopoLogicCore::Vertex>& kpCoreVertex = *coreVertexIterator;
			Vertex^ pVertex = gcnew Vertex(kpCoreVertex);
			pVertices->Add(pVertex);
		}

		return pVertices;
	}

	List<Wire^>^ Edge::Wires()
	{
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopologicalQuery());
		std::list<std::shared_ptr<TopoLogicCore::Wire>> coreWires;
		pCoreEdge->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		for(std::list<std::shared_ptr<TopoLogicCore::Wire>>::iterator coreWireIterator = coreWires.begin();
			coreWireIterator != coreWires.end();
			coreWireIterator++)
		{
			const std::shared_ptr<TopoLogicCore::Wire>& kpCoreWire = *coreWireIterator;
			Wire^ pWire = gcnew Wire(kpCoreWire);
			pWires->Add(pWire);
		}
		
		return pWires;
	}

	Edge^ Edge::ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve)
	{
		return gcnew Edge(curve);
	}

	Edge^ Edge::ByVertices(List<Vertex^>^ vertices)
	{
		std::list<std::shared_ptr<TopoLogicCore::Vertex>> pCoreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			pCoreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(pVertex->GetCoreTopologicalQuery()));
		}
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = TopoLogicCore::Edge::ByVertices(pCoreVertices);
		return gcnew Edge(pCoreEdge);
	}

	Vertex^ Edge::SharedVertex(Edge^ edge)
	{
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(edge->GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Vertex> pCoreVertex = pCoreEdge1->SharedVertex(pCoreEdge2);

		return gcnew Vertex(pCoreVertex);
	}

	Autodesk::DesignScript::Geometry::Curve^ Edge::Curve()
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = BRep_Tool::Curve(TopoDS::Edge(TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(GetCoreTopologicalQuery())->GetOcctShape()), u0, u1);

		return Curve(pOcctCurve, u0, u1);
	}

	double Edge::ParameterAtPoint(Vertex^ vertex)
	{
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopologicalQuery());
		return pCoreEdge->ParameterAtPoint(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(vertex->GetCoreTopologicalQuery()));
	}

	Vertex^ Edge::PointAtParameter(double parameter)
	{
		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopologicalQuery());
		std::shared_ptr<TopoLogicCore::Vertex> pCoreVertex = pCoreEdge->PointAtParameter(parameter);
		return gcnew Vertex(pCoreVertex);
	}

	Object^ Edge::Geometry::get()
	{
		return Curve();
	}

	Autodesk::DesignScript::Geometry::Curve^ Edge::Curve(Handle(Geom_Curve) pOcctCurve, const double u0, const double u1)
	{
		Handle(Geom_BezierCurve) pOcctBezierCurve = Handle_Geom_BezierCurve::DownCast(pOcctCurve);
		if (!pOcctBezierCurve.IsNull())
		{
			throw gcnew NotImplementedException("Feature not yet implemented");
		}

		Handle(Geom_BSplineCurve) pOcctBsplineCurve = Handle_Geom_BSplineCurve::DownCast(pOcctCurve);
		if (!pOcctBsplineCurve.IsNull())
		{
			bool isPeriodic = pOcctBsplineCurve->IsPeriodic();
			if (isPeriodic)
				pOcctBsplineCurve->SetNotPeriodic();

			// Transfer the control points
			const TColgp_Array1OfPnt& rkOcctControlPoints = pOcctBsplineCurve->Poles();
			//int numControlPoints = isPeriodic ? rkOcctControlPoints.Length() + 1 : rkOcctControlPoints.Length();
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
			double occtFirstParameter = pOcctCurve->FirstParameter();
			double occtLastParameter = pOcctCurve->LastParameter();
			double occtDeltaParameter = occtLastParameter - occtFirstParameter;
			if (occtDeltaParameter < Precision::Confusion())
				occtDeltaParameter = Precision::Confusion();
			double dynamoStartParameter = (u0 - occtFirstParameter) / occtDeltaParameter;
			double dynamoEndParameter = (u1 - occtFirstParameter) / occtDeltaParameter;

			int degree = pOcctBsplineCurve->Degree();
			return Autodesk::DesignScript::Geometry::NurbsCurve::ByControlPointsWeightsKnots(pDynamoControlPoints, pWeights, pKnots->ToArray(), degree)
				->TrimByParameter(dynamoStartParameter, dynamoEndParameter);
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
			return Autodesk::DesignScript::Geometry::Circle::ByCenterPointRadiusNormal(
				Autodesk::DesignScript::Geometry::Point::ByCoordinates(rkOcctPosition.Location().X(), rkOcctPosition.Location().Y(), rkOcctPosition.Location().Z()),
				pOcctCircle->Radius(),
				Autodesk::DesignScript::Geometry::Vector::ByCoordinates(rkOcctPosition.Direction().X(), rkOcctPosition.Direction().Y(), rkOcctPosition.Direction().Z())
			);
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

		std::shared_ptr<TopoLogicCore::Edge> pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopologicalQuery());
		List<Vertex^>^ pVertices = Vertices();

		if (pVertices->Count != 2)
		{
			throw gcnew Exception("The edge does not have exactly 2 vertices");
		}

		return Autodesk::DesignScript::Geometry::Line::ByStartPointEndPoint(
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(pVertices[0]->Geometry),
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(pVertices[1]->Geometry)
		);
	}

	std::shared_ptr<TopoLogicCore::TopologicalQuery> Edge::GetCoreTopologicalQuery()
	{
		assert(m_pCoreEdge != nullptr && "Edge::m_pCoreEdge is null.");
		if (m_pCoreEdge == nullptr)
		{
			throw gcnew Exception("Edge::m_pCoreEdge is null.");
		}

		return *m_pCoreEdge;
	}

	Edge::Edge(const std::shared_ptr<TopoLogicCore::Edge>& kpCoreEdge)
		: Topology()
		, m_pCoreEdge(new std::shared_ptr<TopoLogicCore::Edge>(kpCoreEdge))
	{

	}

	Edge::Edge(Autodesk::DesignScript::Geometry::Curve^ curve)
		: Topology()
		, m_pCoreEdge(nullptr)
	{
		if (curve->GetType() == Autodesk::DesignScript::Geometry::Arc::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Circle::typeid)
		{
			Init(safe_cast<Autodesk::DesignScript::Geometry::Circle^>(curve));
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Ellipse::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::EllipseArc::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Helix::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Line::typeid)
		{
			Init(safe_cast<Autodesk::DesignScript::Geometry::Line^>(curve));
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::NurbsCurve::typeid)
		{
			Init(safe_cast<Autodesk::DesignScript::Geometry::NurbsCurve^>(curve));
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::PolyCurve::typeid)
		{
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
			// Please use Wire
		}
		else if (curve->GetType() == Autodesk::DesignScript::Geometry::Curve::typeid) // a generic curve
		{
			Init(curve->ToNurbsCurve());
		}
		else
		{
			throw gcnew ArgumentException("The argument is not a valid Dynamo curve.");
		}
	}

	Edge::~Edge()
	{
		delete m_pCoreEdge;
	}

	void Edge::Init(Autodesk::DesignScript::Geometry::NurbsCurve^ pDynamoNurbsCurve)
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

		m_pCoreEdge = new std::shared_ptr<TopoLogicCore::Edge>(TopoLogicCore::Edge::ByCurve(
			occtPoles,
			occtWeights,
			occtKnots,
			occtMultiplicities,
			degree,
			isPeriodic,
			isRational
			));
	}

	void Edge::Init(Autodesk::DesignScript::Geometry::Circle^ pDynamoCircle)
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

		m_pCoreEdge = new std::shared_ptr<TopoLogicCore::Edge>(
			TopoLogicCore::Edge::ByCurve(pOcctCircle));
	}

	void Edge::Init(Autodesk::DesignScript::Geometry::Line^ pDynamoLine)
	{
		std::list<std::shared_ptr<TopoLogicCore::Vertex>> coreVertices;
		coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>((gcnew Vertex(pDynamoLine->StartPoint))->GetCoreTopologicalQuery()));
		coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>((gcnew Vertex(pDynamoLine->EndPoint))->GetCoreTopologicalQuery()));
		m_pCoreEdge = new std::shared_ptr<TopoLogicCore::Edge>(TopoLogicCore::Edge::ByVertices(coreVertices));
	}
}