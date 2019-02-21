#include <CellComplexFactory.h>
#include <CellComplex.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> CellComplexFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<CellComplex>(TopoDS::CompSolid(rkOcctShape));
	}
}