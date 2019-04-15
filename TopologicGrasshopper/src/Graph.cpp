#include "Graph.h"

namespace TopologicGrasshopper
{
	Graph::Graph()
		: Topologic::Graph()
	{

	}
	Graph::~Graph()
	{

	}
	bool Graph::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Graph::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Graph::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Graph::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Graph::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Graph::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Graph::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Graph::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}