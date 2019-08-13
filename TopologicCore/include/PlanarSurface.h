#pragma once

#include "Utilities.h"
#include "Geometry.h"


namespace TopologicCore
{
	class PlanarSurface : Geometry
	{
	public:
		typedef std::shared_ptr<PlanarSurface> Ptr;

	/*public:
		TOPOLOGIC_API PlanarSurface(Handle(Geom_PlanarSurface) pOcctPlanarSurface);
		virtual ~PlanarSurface();

	protected:
		Handle(Geom_PlanarSurface) m_pOcctPlanarSurface;*/
	};
}