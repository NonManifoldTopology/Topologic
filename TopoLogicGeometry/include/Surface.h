#pragma once

#include <Geom_Surface.hxx>

namespace TopoLogicGeometry
{
	class Surface
	{
	public:
	protected:
		Handle(Geom_Surface) m_pOcctSurface;
	};
}