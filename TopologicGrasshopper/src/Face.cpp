#include "Face.h"

namespace TopologicGrasshopper
{
	Face::Face()
		: Topologic::Face()
	{

	}
	Face::~Face()
	{

	}
	bool Face::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Face::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Face::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Face::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Face::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Face::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Face::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Face::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}