// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

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