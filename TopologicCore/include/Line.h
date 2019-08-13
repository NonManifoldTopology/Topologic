#pragma once

#include "Utilities.h"
#include "Geometry.h"

#include <Geom_Line.hxx>

namespace TopologicCore
{
	class Line : Geometry
	{
	public:
		typedef std::shared_ptr<Line> Ptr;

	public:
		TOPOLOGIC_API Line(Handle(Geom_Line) pOcctLine);
		virtual ~Line();

	protected:
		Handle(Geom_Line) m_pOcctLine;
	};
}