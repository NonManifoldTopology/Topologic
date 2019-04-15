#include "CellComplex.h"

namespace TopologicGrasshopper
{
	CellComplex::CellComplex()
		: Topologic::CellComplex()
	{

	}
	CellComplex::~CellComplex()
	{

	}
	bool CellComplex::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool CellComplex::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ CellComplex::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ CellComplex::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ CellComplex::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool CellComplex::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool CellComplex::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ CellComplex::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}