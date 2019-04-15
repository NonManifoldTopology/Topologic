#pragma once

#include "CellComplex.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class CellComplexParameter : public GH_PersistentParam<TopologicGrasshopper::CellComplex^>
	{
	public:
		CellComplexParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(CellComplex^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<CellComplex^>^% values) override;
	};
}