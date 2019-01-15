#include <GraphFactory.h>
#include <Graph.h>

#include <TopoDS.hxx>

namespace TopologicExtensions
{
	std::shared_ptr<TopologicCore::Topology> GraphFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<DualGraph_>(TopoDS::Compound(rkOcctShape));
	}
}