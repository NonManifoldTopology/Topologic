#pragma once

#include "Face.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class FaceParameter : public GH_PersistentParam<TopologicGrasshopper::Face^>
	{
	public:
		FaceParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Face^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Face^>^% values) override;
	};
}