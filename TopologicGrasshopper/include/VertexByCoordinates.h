#pragma once

using namespace System;
using namespace Grasshopper::Kernel;

namespace TopologicGrasshopper
{
	public ref class VertexByCoordinates : GH_Component
	{
	public:
		VertexByCoordinates();

		property Guid ComponentGuid
		{
			virtual Guid get() override;
		}

	protected:
		virtual void RegisterInputParams(GH_Component::GH_InputParamManager^ pManager) override;
		virtual void RegisterOutputParams(GH_Component::GH_OutputParamManager^ pManager) override;
		virtual void SolveInstance(IGH_DataAccess^ DA) override;
	};
}