#pragma once

#include "Topology.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class TopologyParameter : public GH_PersistentParam<TopologicGrasshopper::Topology^>
	{
	public:
		TopologyParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Topology^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Topology^>^% values) override;
	};
}