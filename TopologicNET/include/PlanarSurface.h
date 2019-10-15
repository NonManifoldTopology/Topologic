// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#pragma once

#include "Surface.h"

#include <TopologicCore/include/PlanarSurface.h>

using namespace System;
using namespace System::Collections::Generic;

/// <summary>
/// </summary>
namespace Topologic {
	ref class NurbsSurface;

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

		NurbsSurface^ ToNurbsSurface();

	public protected:
		PlanarSurface(const std::shared_ptr<TopologicCore::PlanarSurface>& kpCorePlanarSurface);
		virtual ~PlanarSurface();

	protected:
		std::shared_ptr<TopologicCore::PlanarSurface>* m_pCorePlanarSurface;
	};
}