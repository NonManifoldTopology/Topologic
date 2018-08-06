#include <CellFactory.h>
#include <Cell.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> CellFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Cell>(TopoDS::Solid(rkOcctShape));
	}
}