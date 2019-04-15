#pragma once

using namespace System;
using namespace Grasshopper::Kernel::Types;
using namespace Topologic;

namespace TopologicGrasshopper
{
	public ref class Shell : public Topologic::Shell, IGH_Goo
	{
	public:
		Shell();
		virtual ~Shell();
		// Inherited via IGH_Goo
		virtual bool Write(GH_IO::Serialization::GH_IWriter ^writer);
		virtual bool Read(GH_IO::Serialization::GH_IReader ^reader);
		virtual property bool IsValid;
		virtual property System::String ^ IsValidWhyNot;
		virtual property System::String ^ TypeName;
		virtual property System::String ^ TypeDescription;
		virtual Grasshopper::Kernel::Types::IGH_Goo ^ Duplicate();
		virtual System::String ^ ToString() override;
		virtual Grasshopper::Kernel::Types::IGH_GooProxy ^ EmitProxy();
		virtual bool CastFrom(System::Object ^source);
		generic <class T>
		virtual bool CastTo(T %target);
		virtual System::Object ^ ScriptVariable();
	};
}