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

#include <Context.h>
#include <Topology.h>

namespace TopologicCore
{
	std::shared_ptr<Context> Context::ByTopologyParameters(const Topology::Ptr& kpTopology, const double kU, const double kV, const double kW)
	{
		return std::make_shared<Context>(kpTopology, kU, kV, kW);
	}

	Context::Context(const Topology::Ptr& kpTopology, const double kU, double kV, double kW)
		: m_occtShape(kpTopology->GetOcctShape())
		, m_u(kU)
		, m_v(kV)
		, m_w(kW)
	{
	}

	Context::~Context()
	{
	}
	
	std::shared_ptr<Topology> Context::Topology() const
	{
		return Topology::ByOcctShape(m_occtShape, "");
	}
}