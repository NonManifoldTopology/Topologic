#include "NurbsCurve.h"
#include "Vertex.h"

#include <BRep_Tool.hxx>

namespace TopologicCore
{
	NurbsCurve::NurbsCurve(Handle(Geom_BSplineCurve) pOcctBSplineCurve, const TopoDS_Edge& rkOcctEdge)
		: m_pOcctBSplineCurve(pOcctBSplineCurve)
		, m_occtEdge(rkOcctEdge)
	{
	}

	NurbsCurve::~NurbsCurve()
	{
	}

	bool NurbsCurve::IsPeriodic() const
	{
		return m_pOcctBSplineCurve->IsPeriodic();
	}

	bool NurbsCurve::IsRational() const
	{
		return m_pOcctBSplineCurve->IsRational();
	}

	int NurbsCurve::Degree() const
	{
		return m_pOcctBSplineCurve->Degree();
	}

	double NurbsCurve::FirstParameter() const
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = BRep_Tool::Curve(m_occtEdge, u0, u1);
		return u0;
	}

	double NurbsCurve::LastParameter() const
	{
		double u0 = 0.0, u1 = 0.0;
		Handle(Geom_Curve) pOcctCurve = BRep_Tool::Curve(m_occtEdge, u0, u1);
		return u1;
	}

	void NurbsCurve::ControlVertices(std::list<std::shared_ptr<Vertex>>& rControlVertices) const
	{
		TColgp_Array1OfPnt occtControlPoints = m_pOcctBSplineCurve->Poles();
		for (int i = occtControlPoints.Lower(); i <= occtControlPoints.Upper(); i++)
		{
			const gp_Pnt& rkControlPoint = occtControlPoints.Value(i);
			Vertex::Ptr pControlVertex = Vertex::ByCoordinates(rkControlPoint.X(), rkControlPoint.Y(), rkControlPoint.Z());
			rControlVertices.push_back(pControlVertex);
		}
	}

	void NurbsCurve::Knots(std::list<double>& rKnots) const
	{
		const TColStd_Array1OfReal& krOcctKnots = m_pOcctBSplineCurve->Knots();
		for (int i = krOcctKnots.Lower(); i <= krOcctKnots.Upper(); i++)
		{
			int multiplicity = m_pOcctBSplineCurve->Multiplicity(i);
			for (int j = 0; j < multiplicity; j++)
			{
				double occtKnot = krOcctKnots.Value(i);
				rKnots.push_back(occtKnot);
			}
		}
	}
}