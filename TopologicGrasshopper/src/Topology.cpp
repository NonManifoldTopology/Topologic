#include "Topology.h"

namespace TopologicGrasshopper
{
	Topology::Topology()
		: Topologic::Topology()
	{

	}
	Topology::~Topology()
	{

	}
	bool Topology::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Topology::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Topology::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Topology::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Topology::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Topology::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Topology::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Topology::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}