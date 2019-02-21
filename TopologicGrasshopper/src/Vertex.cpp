#include "Vertex.h"

namespace TopologicGrasshopper
{
	Vertex::Vertex()
		: Topologic::Vertex()
	{
		throw gcnew System::NotImplementedException();
	}
	Vertex::~Vertex()
	{

	}
	bool Vertex::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Vertex::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Vertex::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Vertex::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Vertex::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Vertex::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Vertex::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Vertex::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}