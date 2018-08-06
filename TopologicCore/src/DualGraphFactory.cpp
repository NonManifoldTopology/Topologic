#include <DualGraphFactory.h>
#include <DualGraph.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> DualGraphFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<DualGraph>(TopoDS::Wire(rkOcctShape));
	}
}