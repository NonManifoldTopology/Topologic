#include "Line.h"

namespace TopologicCore
{
	Line::Line(Handle(Geom_Line) pOcctLine)
		: m_pOcctLine(pOcctLine)
	{
	}

	Line::~Line()
	{
	}
}