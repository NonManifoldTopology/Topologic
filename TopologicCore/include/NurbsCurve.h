#pragma once

#include "Utilities.h"
#include "Geometry.h"

#include <Geom_BSplineCurve.hxx>
#include <TopoDS_Edge.hxx>

#include <list>

namespace TopologicCore
{
	class Vertex;

	class NurbsCurve : Geometry
	{
	public:
		typedef std::shared_ptr<NurbsCurve> Ptr;

	public:
		TOPOLOGIC_API NurbsCurve(Handle(Geom_BSplineCurve) pOcctBSplineCurve, const TopoDS_Edge& rkOcctEdge);
		virtual ~NurbsCurve();

		TOPOLOGIC_API bool IsPeriodic() const;

		TOPOLOGIC_API bool IsRational() const;

		TOPOLOGIC_API int Degree() const;

		TOPOLOGIC_API void ControlVertices(std::list<std::shared_ptr<Vertex>>& rControlVertices) const;

		TOPOLOGIC_API void Knots(std::list<double>& rKnots) const;

		TOPOLOGIC_API double FirstParameter() const;

		TOPOLOGIC_API double LastParameter() const;

	protected:
		Handle(Geom_BSplineCurve) m_pOcctBSplineCurve;
		TopoDS_Edge m_occtEdge;
	};
}