#include "Cell.h"

namespace TopologicGrasshopper
{
	Cell::Cell()
		: Topologic::Cell()
	{

	}
	Cell::~Cell()
	{

	}
	bool Cell::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Cell::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Cell::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Cell::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Cell::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Cell::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Cell::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Cell::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}