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

#include "NurbsSurface.h"
#include "Vertex.h"

namespace Topologic
{
	NurbsSurface::NurbsSurface(const std::shared_ptr<TopologicCore::NurbsSurface>& kpCoreNurbsSurface)
		: m_pCoreNurbsSurface(kpCoreNurbsSurface != nullptr ? new TopologicCore::NurbsSurface::Ptr(kpCoreNurbsSurface) : throw gcnew Exception("A null Nurbs Curve was created."))
	{

	}

	NurbsSurface::~NurbsSurface()
	{

	}

	int NurbsSurface::UDegree::get()
	{
		return (*m_pCoreNurbsSurface)->UDegree();
	}

	int NurbsSurface::VDegree::get()
	{
		return (*m_pCoreNurbsSurface)->VDegree();
	}

	bool NurbsSurface::IsURational::get()
	{
		return (*m_pCoreNurbsSurface)->IsURational();
	}

	bool NurbsSurface::IsVRational::get()
	{
		return (*m_pCoreNurbsSurface)->IsVRational();
	}

	int NurbsSurface::NumOfUControlVertices::get()
	{
		return (*m_pCoreNurbsSurface)->NumOfUControlVertices();
	}

	int NurbsSurface::NumOfVControlVertices::get()
	{
		return (*m_pCoreNurbsSurface)->NumOfVControlVertices();
	}

	Vertex^ NurbsSurface::ControlVertex(int u, int v)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = (*m_pCoreNurbsSurface)->ControlVertex(u, v);
		Vertex^ vertex = safe_cast<Vertex^>(Topology::ByCoreTopology(pCoreVertex));
		return vertex;
	}

	List<double>^ NurbsSurface::UKnots::get()
	{
		std::list<double> coreUKnots;
		(*m_pCoreNurbsSurface)->UKnots(coreUKnots);

		List<double>^ uKnots = gcnew List<double>();
		for (const double kUKnot : coreUKnots)
		{
			uKnots->Add(kUKnot);
		}
		return uKnots;
	}

	List<double>^ NurbsSurface::VKnots::get()
	{
		std::list<double> coreVKnots;
		(*m_pCoreNurbsSurface)->VKnots(coreVKnots);

		List<double>^ vKnots = gcnew List<double>();
		for (const double kVKnot : coreVKnots)
		{
			vKnots->Add(kVKnot);
		}
		return vKnots;
	}
	/*bool NurbsSurface::IsPeriodic::get()
	{
		return (*m_pCoreNurbsSurface)->IsPeriodic();
	}*/
}