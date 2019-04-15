#include "Wire.h"

namespace TopologicGrasshopper
{
	Wire::Wire()
		: Topologic::Wire()
	{

	}
	Wire::~Wire()
	{

	}
	bool Wire::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Wire::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Wire::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Wire::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Wire::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Wire::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Wire::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Wire::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}