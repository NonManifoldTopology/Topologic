#pragma once

#include "Wire.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class WireParameter : public GH_PersistentParam<TopologicGrasshopper::Wire^>
	{
	public:
		WireParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Wire^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Wire^>^% values) override;
	};
}