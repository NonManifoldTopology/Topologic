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

#include <TopoDS_Shape.hxx>

#ifdef TOPOLOGIC_EXPORTS
#define TOPOLOGIC_API __declspec(dllexport)
#else
#define TOPOLOGIC_API __declspec(dllimport)
#endif

namespace TopologicCore {
	enum TopologyType
	{
		TOPOLOGY_VERTEX = 1,
		TOPOLOGY_EDGE = 2,
		TOPOLOGY_WIRE = 4,
		TOPOLOGY_FACE = 8,
		TOPOLOGY_SHELL = 16,
		TOPOLOGY_CELL = 32,
		TOPOLOGY_CELLCOMPLEX = 64,
		TOPOLOGY_CLUSTER = 128,
		TOPOLOGY_APERTURE = 256,
		TOPOLOGY_ALL = 511
	};

	struct OcctShapeComparator {
		bool operator()(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2) const {
			int value1 = (int)ptrdiff_t(rkOcctShape1.TShape().operator->());
			int value2 = (int)ptrdiff_t(rkOcctShape2.TShape().operator->());
			return value1 < value2;
		}
	};

	TopAbs_ShapeEnum OcctTypeFromTopologicType(const TopologyType kTopologyType);
}