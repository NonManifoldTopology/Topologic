#include <ClusterFactory.h>
#include <Cluster.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> ClusterFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Cluster>(TopoDS::Compound(rkOcctShape));
	}
}