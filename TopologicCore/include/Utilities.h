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