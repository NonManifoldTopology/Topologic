#pragma once

#include "Geometry.h"

namespace TopologicCore
{
	class Surface
	{
	public:
		typedef std::shared_ptr<Surface> Ptr;

	public:
		virtual ~Surface() = 0;
	};
}