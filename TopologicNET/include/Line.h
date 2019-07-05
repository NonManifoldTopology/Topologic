#pragma once

#include "Geometry.h"

#include <TopologicCore/include/Line.h>

using namespace System;

/// <summary>
/// </summary>
namespace Topologic {
	/// <summary>
	/// </summary>
	public ref class Line : Geometry
	{
	public:
		void A() {}
	public protected:
		Line(const std::shared_ptr<TopologicCore::Line>& kpCoreLine);
		virtual ~Line();

	protected:
		std::shared_ptr<TopologicCore::Line>* m_pCoreLine;
	};
}