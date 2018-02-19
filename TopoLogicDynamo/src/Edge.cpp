#include <Edge.h>
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
	Dictionary<String^, Object^>^ Edge::StartVertex(Edge^ topoLogicEdge)
	{
		TopoLogicCore::Edge* pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(topoLogicEdge->GetCoreTopology());
		Vertex^ pVertex = gcnew Vertex(pCoreEdge->StartVertex());
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertex", pVertex);
		pDictionary->Add("Point", pVertex->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Edge::EndVertex(Edge^ topoLogicEdge)
	{
		TopoLogicCore::Edge* pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(topoLogicEdge->GetCoreTopology());
		Vertex^ pVertex = gcnew Vertex(pCoreEdge->EndVertex());
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertex", pVertex);
		pDictionary->Add("Point", pVertex->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Edge::Wires(Edge ^ topoLogicEdge)
	{
		TopoLogicCore::Edge* pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(topoLogicEdge->GetCoreTopology());
		std::list<TopoLogicCore::Wire*> coreWires;
		pCoreEdge->Wires(coreWires);

		List<Wire^>^ pWires = gcnew List<Wire^>();
		List<Object^>^ pDynamoPolycurves = gcnew List<Object^>();
		for(std::list<TopoLogicCore::Wire*>::iterator coreWireIterator = coreWires.begin();
			coreWireIterator != coreWires.end();
			coreWireIterator++)
		{
			TopoLogicCore::Wire* pCoreWire = *coreWireIterator;
			Wire^ pWire = gcnew Wire(pCoreWire);
			pWires->Add(pWire);
			pDynamoPolycurves->Add(pWire->Geometry);
		}
		
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Wires", pWires);
		pDictionary->Add("PolyCurves", pDynamoPolycurves);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Edge::ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve)
	{
		Edge^ pEdge = gcnew Edge(curve);
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edge", pEdge);
		pDictionary->Add("Curve", pEdge->Geometry);
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Edge::ByVertices(List<Vertex^>^ vertices)
	{
		std::list<TopoLogicCore::Vertex*> pCoreVertices;
		for each(Vertex^ pVertex in vertices)
		{
			pCoreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>(pVertex->GetCoreTopology()));
		}
		TopoLogicCore::Edge* pCoreEdge = TopoLogicCore::Edge::ByVertices(pCoreVertices);
		Edge^ pEdge = gcnew Edge(pCoreEdge);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Edge", pEdge);
		pDictionary->Add("Line", pEdge->Geometry);

		return pDictionary;
	}

	Dictionary<String^, Object^>^ Edge::SharedVertex(Edge^ topoLogicEdge1, Edge^ topoLogicEdge2)
	{
		TopoLogicCore::Edge* pCoreEdge1 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(topoLogicEdge1->GetCoreTopology());
		TopoLogicCore::Edge* pCoreEdge2 = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(topoLogicEdge2->GetCoreTopology());
		TopoLogicCore::Vertex* pCoreVertex = pCoreEdge1->SharedVertex(pCoreEdge2);

		Vertex^ pVertex = gcnew Vertex(pCoreVertex);

		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		pDictionary->Add("TopoLogic Vertex", pVertex);
		pDictionary->Add("Point", pVertex->Geometry);
		return pDictionary;
	}

	Autodesk::DesignScript::Geometry::Curve^ Edge::Curve()
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = BRep_Tool::Curve(TopoDS::Edge(*GetCoreTopology()->GetOcctShape()), u0, u1);

		return Curve(pOcctCurve, u0, u1);
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
			throw gcnew NotImplementedException("Feature not yet implemented");
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

		TopoLogicCore::Edge* pCoreEdge = TopoLogicCore::Topology::Downcast<TopoLogicCore::Edge>(GetCoreTopology());
		Vertex^ pStartVertex = gcnew Vertex(pCoreEdge->StartVertex());
		Vertex^ pEndVertex = gcnew Vertex(pCoreEdge->EndVertex());

		return Autodesk::DesignScript::Geometry::Line::ByStartPointEndPoint(
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(pStartVertex->Geometry),
			safe_cast<Autodesk::DesignScript::Geometry::Point^>(pEndVertex->Geometry)
		);
	}

	TopoLogicCore::Topology* Edge::GetCoreTopology()
	{
		assert(m_pCoreEdge != nullptr && "Edge::m_pCoreEdge is null.");
		if (m_pCoreEdge == nullptr)
		{
			throw gcnew Exception("Edge::m_pCoreEdge is null.");
		}

		return m_pCoreEdge;
	}

	Edge::Edge(TopoLogicCore::Edge * const kpCoreEdge)
		: Topology()
		, m_pCoreEdge(kpCoreEdge)
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
			throw gcnew System::NotImplementedException("Feature not yet implemented.");
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

		m_pCoreEdge = TopoLogicCore::Edge::ByCurve(
			occtPoles,
			occtWeights,
			occtKnots,
			occtMultiplicities,
			degree,
			isPeriodic,
			isRational
			);
	}

	void Edge::Init(Autodesk::DesignScript::Geometry::Line^ pDynamoLine)
	{
		std::list<TopoLogicCore::Vertex*> coreVertices;
		coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>((gcnew Vertex(pDynamoLine->StartPoint))->GetCoreTopology()));
		coreVertices.push_back(TopoLogicCore::Topology::Downcast<TopoLogicCore::Vertex>((gcnew Vertex(pDynamoLine->EndPoint))->GetCoreTopology()));
		m_pCoreEdge = TopoLogicCore::Edge::ByVertices(coreVertices);
	}
}