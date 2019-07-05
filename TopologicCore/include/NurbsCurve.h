#pragma once

#include "Utilities.h"
#include "Geometry.h"

#include <Geom_BSplineCurve.hxx>

#include <list>

namespace TopologicCore
{
	class Vertex;

	class NurbsCurve : Geometry
	{
	public:
		typedef std::shared_ptr<NurbsCurve> Ptr;

	public:
		TOPOLOGIC_API NurbsCurve(Handle(Geom_BSplineCurve) pOcctBSplineCurve);
		virtual ~NurbsCurve();

		TOPOLOGIC_API bool IsPeriodic() const;

		TOPOLOGIC_API bool IsRational() const;

		TOPOLOGIC_API int Degree() const;

		TOPOLOGIC_API void ControlVertices(std::list<std::shared_ptr<Vertex>>& rControlVertices) const;

		TOPOLOGIC_API void Knots(std::list<double>& rKnots) const;

	protected:
		Handle(Geom_BSplineCurve) m_pOcctBSplineCurve;
	};
}