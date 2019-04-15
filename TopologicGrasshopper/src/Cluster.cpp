#include "Cluster.h"

namespace TopologicGrasshopper
{
	Cluster::Cluster()
		: Topologic::Cluster()
	{

	}
	Cluster::~Cluster()
	{

	}
	bool Cluster::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Cluster::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Cluster::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Cluster::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Cluster::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Cluster::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Cluster::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Cluster::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}