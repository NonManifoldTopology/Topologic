#include "Edge.h"

namespace TopologicGrasshopper
{
	Edge::Edge()
		: Topologic::Edge()
	{

	}
	Edge::~Edge()
	{

	}
	bool Edge::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Edge::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Edge::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Edge::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Edge::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Edge::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Edge::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Edge::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}