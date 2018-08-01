#pragma once

#include <TopoDS_Shape.hxx>

#ifdef TOPOLOGIC_EXPORTS
#define TOPOLOGIC_API __declspec(dllexport)
#else
#define TOPOLOGIC_API __declspec(dllimport)
#endif


struct OcctShapeComparator {
	bool operator()(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2) const {
		int upperLimit = std::numeric_limits<int>::max();
		return rkOcctShape1.HashCode(upperLimit) < rkOcctShape2.HashCode(upperLimit);
	}
};