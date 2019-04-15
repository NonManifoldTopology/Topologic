#pragma once

#include "Edge.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class EdgeParameter : public GH_PersistentParam<TopologicGrasshopper::Edge^>
	{
	public:
		EdgeParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Edge^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Edge^>^% values) override;
	};
}