#pragma once

#include "Graph.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class GraphParameter : public GH_PersistentParam<TopologicGrasshopper::Graph^>
	{
	public:
		GraphParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Graph^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Graph^>^% values) override;
	};
}