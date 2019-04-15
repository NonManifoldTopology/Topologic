#include "VertexByCoordinates.h"
#include "VertexParameter.h"

namespace TopologicGrasshopper
{
	VertexByCoordinates::VertexByCoordinates()
		:GH_Component("VertexByCoordinates", "VertexByCoordinates", "Creates a Vertex ByCoordinates", "Topologic", "Vertex")
	{

	}

	void VertexByCoordinates::RegisterInputParams(GH_Component::GH_InputParamManager ^ pManager)
	{
		pManager->AddNumberParameter("X", "X", "X coordinate", GH_ParamAccess::item);
		pManager->AddNumberParameter("Y", "Y", "Y coordinate", GH_ParamAccess::item);
		pManager->AddNumberParameter("Z", "Z", "Z coordinate", GH_ParamAccess::item);
	}

	void VertexByCoordinates::RegisterOutputParams(GH_Component::GH_OutputParamManager ^ pManager)
	{
		pManager->AddParameter(gcnew VertexParameter(), "Vertex", "Vertex", "A Vertex", GH_ParamAccess::item);
	}

	void VertexByCoordinates::SolveInstance(IGH_DataAccess ^ DA)
	{

	}

	Guid VertexByCoordinates::ComponentGuid::get()
	{
		return Guid("2655a8ef-a473-4b8c-8b2a-c0da378b5528");
	}
}