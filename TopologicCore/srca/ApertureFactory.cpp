#include <ApertureFactory.h>
#include <Aperture.h>
#include <Context.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> ApertureFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		Topology::Ptr pTopology = Topology::ByOcctShape(rkOcctShape, "");
		Context::Ptr pContext = nullptr;
		bool defaultOpenStatus = false;
		return std::make_shared<Aperture>(pTopology, pContext, defaultOpenStatus);
	}
}