#pragma once

#include "Aperture.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class ApertureParameter : public GH_PersistentParam<TopologicGrasshopper::Aperture^>
	{
	public:
		ApertureParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Aperture^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Aperture^>^% values) override;
	};
}