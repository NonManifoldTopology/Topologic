#pragma once

#include "Utilities.h"

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