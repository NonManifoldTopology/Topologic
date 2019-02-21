#include <FaceFactory.h>
#include <Face.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> FaceFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Face>(TopoDS::Face(rkOcctShape));
	}
}