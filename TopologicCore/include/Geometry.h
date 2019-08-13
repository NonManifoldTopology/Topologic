#pragma once

#include "Utilities.h"

#include <Geom_BSplineCurve.hxx>

namespace TopologicCore
{
	class Geometry
	{
	public:
		typedef std::shared_ptr<Geometry> Ptr;

	public:

		virtual ~Geometry() = 0;
	};
}