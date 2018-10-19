#pragma once

#include <TopoDS_Shape.hxx>

#ifdef TOPOLOGIC_EXPORTS
#define TOPOLOGIC_API __declspec(dllexport)
#else
#define TOPOLOGIC_API __declspec(dllimport)
#endif

namespace TopologicCore {
	struct OcctShapeComparator {
		bool operator()(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2) const {
			int value1 = (int)ptrdiff_t(rkOcctShape1.TShape().operator->());
			int value2 = (int)ptrdiff_t(rkOcctShape2.TShape().operator->());
			return value1 < value2;
		}
	};
}