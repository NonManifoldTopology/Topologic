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
// along with this program. If not, see <https://www.gnu.org/licenses/>.

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