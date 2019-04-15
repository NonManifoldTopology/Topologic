#pragma once

#include "Context.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class ContextParameter : public GH_PersistentParam<TopologicGrasshopper::Context^>
	{
	public:
		ContextParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Context^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Context^>^% values) override;
	};
}