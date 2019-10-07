#include "PlanarSurface.h"

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
}