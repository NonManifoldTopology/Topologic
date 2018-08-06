#include <VertexFactory.h>
#include <Vertex.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> VertexFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Vertex>(TopoDS::Vertex(rkOcctShape));
	}
}