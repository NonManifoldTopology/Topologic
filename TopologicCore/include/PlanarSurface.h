#pragma once

#include "Utilities.h"
#include "Geometry.h"

#include <Geom_Plane.hxx>
#include <TopoDS_Face.hxx>

#include <list>

namespace TopologicCore
{
	class NurbsSurface;

	class PlanarSurface : Geometry
	{
	public:
		typedef std::shared_ptr<PlanarSurface> Ptr;

	public:
		TOPOLOGIC_API PlanarSurface(Handle(Geom_Plane) pOcctPlane, const TopoDS_Face& rkOcctFace);
		virtual ~PlanarSurface();

		TOPOLOGIC_API void Coefficients(std::list<double>& rCoefficients) const;

		TOPOLOGIC_API double XMin() const;
		TOPOLOGIC_API double XMax() const;
		TOPOLOGIC_API double YMin() const;
		TOPOLOGIC_API double YMax() const;

		TOPOLOGIC_API std::shared_ptr<NurbsSurface> ToNurbsSurface() const;

	protected:
		Handle(Geom_Plane) m_pOcctPlane;
		TopoDS_Face m_occtFace;
	};
}