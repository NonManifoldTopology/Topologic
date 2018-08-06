#include <ShellFactory.h>
#include <Shell.h>

#include <TopoDS.hxx>

namespace TopologicCore
{
	std::shared_ptr<Topology> ShellFactory::Create(const TopoDS_Shape& rkOcctShape)
	{
		return std::make_shared<Shell>(TopoDS::Shell(rkOcctShape));
	}
}