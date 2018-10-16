#include <GraphFactory.h>
#include <Graph.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> GraphFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Graph>(TopoDS::Wire(rkOcctShape));
	}
}