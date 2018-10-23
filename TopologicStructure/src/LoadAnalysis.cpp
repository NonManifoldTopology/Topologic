#include <LoadAnalysis.h>

#include <OcctLoadAttribute.h>

#include <Standard_GUID.hxx>

namespace TopologicStructure
{
	List<Load^>^ LoadAnalysis::Loads(Topologic::Topology ^ topology)
	{
		List<Topologic::Topology^>^ contentTopologies = topology->Contents(true);
		List<Load^>^ loads = gcnew List<Load^>();
		//for each (Topologic::Topology^ contentTopology in contentTopologies)
		//{
		//	char strID[36] = "";
		//	OcctLoadAttribute::GetID().ToCString(strID);
		//	TDF_Attribute* pAttribute = contentTopology->FindAttribute(gcnew String(strID));
		//	if (pAttribute == nullptr)
		//	{
		//		continue;
		//	}

		//	// Do NOT delete these; they are from the smart pointers in OCAF.
		//	OcctLoadAttribute* pLoadAttribute = dynamic_cast<OcctLoadAttribute*>(pAttribute);
		//	if(pLoadAttribute != nullptr)
		//	{
		//		Topologic::Vertex^ vertex = safe_cast<Topologic::Vertex^>(contentTopology);
		//		TopologicUtility::Vector::Ptr vector = TopologicUtility::Vector::ByCoordinates(pLoadAttribute->X(), pLoadAttribute->Y(), pLoadAttribute->Z());
		//		Load^ load = gcnew Load(vertex, vector, false);
		//		loads->Add(load);
		//	}
		//}
		return loads;
	}
}