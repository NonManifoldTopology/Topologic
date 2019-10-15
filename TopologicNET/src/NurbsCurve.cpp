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

#include "NurbsCurve.h"
#include "Vertex.h"

namespace Topologic
{
	NurbsCurve::NurbsCurve(const std::shared_ptr<TopologicCore::NurbsCurve>& kpCoreNurbsCurve)
		: m_pCoreNurbsCurve(kpCoreNurbsCurve != nullptr ? new TopologicCore::NurbsCurve::Ptr(kpCoreNurbsCurve) : throw gcnew Exception("A null Nurbs Curve was created."))
	{

	}

	NurbsCurve::~NurbsCurve()
	{

	}

	bool NurbsCurve::IsPeriodic::get()
	{
		return (*m_pCoreNurbsCurve)->IsPeriodic();
	}

	bool NurbsCurve::IsRational::get()
	{
		return (*m_pCoreNurbsCurve)->IsRational();
	}

	int NurbsCurve::Degree::get()
	{
		return (*m_pCoreNurbsCurve)->Degree();
	}

	List<Vertex^>^ NurbsCurve::ControlVertices::get()
	{
		std::list<TopologicCore::Vertex::Ptr> coreControlVertices;
		(*m_pCoreNurbsCurve)->ControlVertices(coreControlVertices);

		List<Vertex^>^ controlVertices = gcnew List<Vertex^>();
		for (const TopologicCore::Vertex::Ptr kpCoreControlVertex : coreControlVertices)
		{
			Vertex^ vertex = safe_cast<Vertex^>(Topology::ByCoreTopology(kpCoreControlVertex));
			controlVertices->Add(vertex);
		}
		return controlVertices;
	}

	List<double>^ NurbsCurve::Knots::get()
	{
		std::list<double> coreKnots;
		(*m_pCoreNurbsCurve)->Knots(coreKnots);

		List<double>^ knots = gcnew List<double>();
		for (const double kKnot : coreKnots)
		{
			knots->Add(kKnot);
		}
		return knots;
	}

	double NurbsCurve::FirstParameter::get()
	{
		return (*m_pCoreNurbsCurve)->FirstParameter();
	}

	double NurbsCurve::LastParameter::get()
	{
		return (*m_pCoreNurbsCurve)->LastParameter();
	}
}