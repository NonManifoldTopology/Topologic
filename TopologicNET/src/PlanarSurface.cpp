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

#include "PlanarSurface.h"
#include "NurbsSurface.h"

#include <list>

namespace Topologic
{
	PlanarSurface::PlanarSurface(const std::shared_ptr<TopologicCore::PlanarSurface>& kpCorePlanarSurface)
		: m_pCorePlanarSurface(kpCorePlanarSurface != nullptr ? new TopologicCore::PlanarSurface::Ptr(kpCorePlanarSurface) : throw gcnew Exception("A null Nurbs Curve was created."))
	{

	}

	PlanarSurface::~PlanarSurface()
	{

	}

	List<double>^ PlanarSurface::Coefficients::get()
	{
		List<double>^ coefficients = gcnew List<double>();

		std::list<double> coreCoefficients;
		(*m_pCorePlanarSurface)->Coefficients(coreCoefficients);
		for (const double kCoefficient : coreCoefficients)
		{
			coefficients->Add(kCoefficient);
		}

		return coefficients;
	}

	double PlanarSurface::XMin::get()
	{
		return (*m_pCorePlanarSurface)->XMin();
	}

	double PlanarSurface::YMin::get()
	{
		return (*m_pCorePlanarSurface)->YMin();
	}

	double PlanarSurface::XMax::get()
	{
		return (*m_pCorePlanarSurface)->XMax();
	}

	double PlanarSurface::YMax::get()
	{
		return (*m_pCorePlanarSurface)->YMax();
	}

	NurbsSurface^ PlanarSurface::ToNurbsSurface()
	{
		try {
			return gcnew NurbsSurface((*m_pCorePlanarSurface)->ToNurbsSurface());
		}
		catch (const std::exception& e)
		{
			throw gcnew Exception(gcnew String(e.what()));
		}
	}
}