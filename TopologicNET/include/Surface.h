#pragma once

#include <memory>

#include "Geometry.h"

using namespace System;

/// <summary>
/// </summary>
namespace Topologic {
	/// <summary>
	/// </summary>
	public ref class Surface abstract : Geometry
	{

	protected:
		Surface();
		virtual ~Surface();
	};
}