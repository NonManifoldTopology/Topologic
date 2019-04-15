#include "Context.h"

namespace TopologicGrasshopper
{
	Context::Context()
		: Topologic::Context()
	{

	}
	Context::~Context()
	{

	}
	bool Context::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Context::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Context::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Context::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Context::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Context::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Context::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Context::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}