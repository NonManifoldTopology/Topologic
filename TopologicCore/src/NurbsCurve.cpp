#include "NurbsCurve.h"
#include "Vertex.h"

namespace TopologicCore
{
	NurbsCurve::NurbsCurve(Handle(Geom_BSplineCurve) pOcctBSplineCurve)
		: m_pOcctBSplineCurve(pOcctBSplineCurve)
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