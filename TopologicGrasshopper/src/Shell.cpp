#include "Shell.h"

namespace TopologicGrasshopper
{
	Shell::Shell()
		: Topologic::Shell()
	{

	}
	Shell::~Shell()
	{

	}
	bool Shell::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Shell::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Shell::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Shell::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Shell::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Shell::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Shell::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Shell::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}