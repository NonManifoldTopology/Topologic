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

#include "Utilities.h"

namespace TopologicCore {
	TopAbs_ShapeEnum OcctTypeFromTopologicType(const TopologyType kTopologyType)
	{
		switch (kTopologyType)
		{
		case TOPOLOGY_VERTEX: return TopAbs_VERTEX;
		case TOPOLOGY_EDGE: return TopAbs_EDGE;
		case TOPOLOGY_WIRE: return TopAbs_WIRE;
		case TOPOLOGY_FACE: return TopAbs_FACE;
		case TOPOLOGY_SHELL: return TopAbs_SHELL;
		case TOPOLOGY_CELL: return TopAbs_SOLID;
		case TOPOLOGY_CELLCOMPLEX: return TopAbs_COMPSOLID;
		case TOPOLOGY_CLUSTER: return TopAbs_COMPOUND;
		default: throw std::exception("No OCCT counterpart is found from this topology.");
		}
	}
}