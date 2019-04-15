#pragma once

#include "Cluster.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class ClusterParameter : public GH_PersistentParam<TopologicGrasshopper::Cluster^>
	{
	public:
		ClusterParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Cluster^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Cluster^>^% values) override;
	};
}