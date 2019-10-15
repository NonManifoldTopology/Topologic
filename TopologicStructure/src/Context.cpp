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