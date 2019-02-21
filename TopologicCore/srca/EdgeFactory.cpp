#include <EdgeFactory.h>
#include <Edge.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> EdgeFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Edge>(TopoDS::Edge(rkOcctShape));
	}
}