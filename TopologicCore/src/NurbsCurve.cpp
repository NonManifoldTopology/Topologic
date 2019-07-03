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
}