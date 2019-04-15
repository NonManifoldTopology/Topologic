#include "Vertex.h"
#include "VertexParameter.h"

namespace TopologicGrasshopper
{
	VertexParameter::VertexParameter()
		: GH_PersistentParam<TopologicGrasshopper::Vertex^>("VertexParameter", "VertexParameter", "Represents a Vertex paraneter", "Topologic", "Vertex")
	{

	}

	GH_GetterResult VertexParameter::Prompt_Singular(Vertex^% value)
	{
		return GH_GetterResult::cancel;
	}

	GH_GetterResult VertexParameter::Prompt_Plural(List<Vertex^>^% values)
	{
		return GH_GetterResult::cancel;
	}

	Guid VertexParameter::ComponentGuid::get()
	{
		return Guid("5d11af3e-f316-46db-94aa-11e10e39b17a");
	}
}