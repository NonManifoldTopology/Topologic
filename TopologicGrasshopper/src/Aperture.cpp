#include "Aperture.h"

namespace TopologicGrasshopper
{
	Aperture::Aperture()
		: Topologic::Aperture()
	{

	}
	Aperture::~Aperture()
	{

	}
	bool Aperture::Write(GH_IO::Serialization::GH_IWriter ^writer)
	{
		return false;
	}
	bool Aperture::Read(GH_IO::Serialization::GH_IReader ^reader)
	{
		return false;
	}
	Grasshopper::Kernel::Types::IGH_Goo ^ Aperture::Duplicate()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	System::String ^ Aperture::ToString()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	Grasshopper::Kernel::Types::IGH_GooProxy ^ Aperture::EmitProxy()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
	bool Aperture::CastFrom(System::Object ^source)
	{
		return false;
	}

	generic <class T>
	bool Aperture::CastTo(T %target)
	{
		return false;
	}
	System::Object ^ Aperture::ScriptVariable()
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}
}