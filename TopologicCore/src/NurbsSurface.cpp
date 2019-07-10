#include "NurbsSurface.h"
#include "Vertex.h"
#include "Face.h"

namespace TopologicCore
{
	NurbsSurface::NurbsSurface(Handle(Geom_BSplineSurface) pOcctBSplineSurface, const std::shared_ptr<Face> kpFace)
		: m_pOcctBSplineSurface(pOcctBSplineSurface)
		, m_pFace(kpFace)
	{
	}

	NurbsSurface::~NurbsSurface()
	{
	}

	int NurbsSurface::UDegree() const
	{
		return m_pOcctBSplineSurface->UDegree();
	}

	int NurbsSurface::VDegree() const
	{
		return m_pOcctBSplineSurface->VDegree();
	}

	bool NurbsSurface::IsURational() const
	{
		return m_pOcctBSplineSurface->IsURational();
	}

	bool NurbsSurface::IsVRational() const
	{
		return m_pOcctBSplineSurface->IsVRational();
	}

	int NurbsSurface::NumOfUControlVertices() const
	{
		return m_pOcctBSplineSurface->NbUPoles();
	}

	int NurbsSurface::NumOfVControlVertices() const
	{
		return m_pOcctBSplineSurface->NbVPoles();
	}

	/*bool NurbsSurface::IsPeriodic() const
	{
		return m_pOcctBSplineSurface->IsPeriodic();
	}*/

	void NurbsSurface::ControlVertices(std::vector<std::shared_ptr<Vertex>>& rControlVertices, int& rUCount, int& rVCount) const
	{
		/*TColgp_Array2OfPnt occtControlPoints = m_pOcctBSplineSurface->Poles();
		for (int i = occtControlPoints.Lower(); i <= occtControlPoints.Upper(); i++)
		{
			const gp_Pnt& rkControlPoint = occtControlPoints.Value(i);
			Vertex::Ptr pControlVertex = Vertex::ByCoordinates(rkControlPoint.X(), rkControlPoint.Y(), rkControlPoint.Z());
			rControlVertices.push_back(pControlVertex);
		}*/
	}

	Vertex::Ptr NurbsSurface::ControlVertex(const int u, const int v) const
	{
		const gp_Pnt& rkControlPoint = m_pOcctBSplineSurface->Poles().Value(
			m_pOcctBSplineSurface->Poles().LowerRow() + u,
			m_pOcctBSplineSurface->Poles().LowerCol() + v);
		Vertex::Ptr vertex = Vertex::ByCoordinates(rkControlPoint.X(), rkControlPoint.Y(), rkControlPoint.Z());
		return vertex;
	}

	void NurbsSurface::UKnots(std::list<double>& rUKnots) const
	{
		const TColStd_Array1OfReal& krOcctUKnots = m_pOcctBSplineSurface->UKnots();
		for (int i = krOcctUKnots.Lower(); i <= krOcctUKnots.Upper(); i++)
		{
			int uMultiplicity = m_pOcctBSplineSurface->UMultiplicity(i);
			for (int j = 0; j < uMultiplicity; j++)
			{
				double occtUKnot = krOcctUKnots.Value(i);
				rUKnots.push_back(occtUKnot);
			}
		}
	}

	void NurbsSurface::VKnots(std::list<double>& rVKnots) const
	{
		const TColStd_Array1OfReal& krOcctVKnots = m_pOcctBSplineSurface->VKnots();
		for (int i = krOcctVKnots.Lower(); i <= krOcctVKnots.Upper(); i++)
		{
			int vMultiplicity = m_pOcctBSplineSurface->VMultiplicity(i);
			for (int j = 0; j < vMultiplicity; j++)
			{
				double occtVKnot = krOcctVKnots.Value(i);
				rVKnots.push_back(occtVKnot);
			}
		}
	}
}