#include <WireFactory.h>
#include <Wire.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> WireFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Wire>(TopoDS::Wire(rkOcctShape));
	}
}