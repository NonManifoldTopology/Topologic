#pragma once

#include "Shell.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class ShellParameter : public GH_PersistentParam<TopologicGrasshopper::Shell^>
	{
	public:
		ShellParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Shell^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Shell^>^% values) override;
	};
}