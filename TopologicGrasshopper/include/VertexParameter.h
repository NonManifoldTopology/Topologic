#pragma once

#include "Vertex.h"

using namespace System;
using namespace System::Collections::Generic;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class VertexParameter : public GH_PersistentParam<TopologicGrasshopper::Vertex^>
	{
	public:
		VertexParameter();

		property Guid ComponentGuid
		{
			// Always generate a new Guid, but never change it once 
			// you've released this parameter to the public.
			virtual Guid get() override;
		}

	protected:
		virtual GH_GetterResult Prompt_Singular(Vertex^% value) override;
		virtual GH_GetterResult Prompt_Plural(List<Vertex^>^% values) override;
	};
}