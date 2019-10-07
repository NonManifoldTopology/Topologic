#pragma once

#include "Surface.h"

#include <TopologicCore/include/PlanarSurface.h>

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// </summary>
namespace Topologic {
	/// <summary>
	/// </summary>
	public ref class PlanarSurface : Surface
	{
	public:
		property List<double>^ Coefficients
		{
			List<double>^ get();
		}

		property double XMin
		{
			double get();
		}

		property double YMin
		{
			double get();
		}

		property double XMax
		{
			double get();
		}

		property double YMax
		{
			double get();
		}

	public protected:
		PlanarSurface(const std::shared_ptr<TopologicCore::PlanarSurface>& kpCorePlanarSurface);
		virtual ~PlanarSurface();

	protected:
		std::shared_ptr<TopologicCore::PlanarSurface>* m_pCorePlanarSurface;
	};
}