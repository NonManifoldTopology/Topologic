#pragma once

#include "Cell.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class CellParameter : public GH_PersistentParam<TopologicGrasshopper::Cell^>
	{
	public:
		CellParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Cell^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Cell^>^% values) override;
	};
}