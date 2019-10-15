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

namespace TopologicStructure 
{
	Context^ Context::Create(Topology^ hostTopology, double u, double v, double w, double uScale, double vScale, double wScale, double uShift, double vShift, double wShift)
	{
		return gcnew Context(hostTopology, u, v, w, uScale, vScale, wScale, uShift, vShift, wShift);
	}

	Context::Context(Topology ^ hostTopology, double u, double v, double w, double uScale, double vScale, double wScale, double uShift, double vShift, double wShift)
		: m_pHostTopology(hostTopology)
		, m_u(u)
		, m_v(v)
		, m_w(w)
		, m_uScale(uScale)
		, m_vScale(vScale)
		, m_wScale(wScale)
		, m_uShift(uShift)
		, m_vShift(vShift)
		, m_wShift(wShift)
	{

	}

	Context::~Context()
	{

	}
}