#include <LoadFactory.h>
#include <Load.h>

#include <TopologicCore/include/Vertex.h>

namespace TopologicStructure
{
	//Topologic::Topology^ LoadFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	//{
	//	throw gcnew System::NotImplementedException();
	//	/*TopologicCore::Vertex::Ptr pCoreVertex = std::dynamic_pointer_cast<TopologicCore::Vertex>(kpTopology);
	//	if (pCoreVertex == nullptr)
	//	{
	//		return nullptr;
	//	}
	//	return gcnew Load()*/
	//}
	//Topologic::Topology ^ LoadFactory::Create(TopologicCore::Topology const * const kpTopology)
	//{
	//	throw gcnew System::NotImplementedException();
	//	// TODO: insert return statement here
	//}

	Topologic::Topology ^ LoadFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = std::dynamic_pointer_cast<TopologicCore::Vertex>(kpTopology.topologyPtr);
		if (pCoreVertex == nullptr)
		{
			return nullptr;
		}
		return gcnew Load(pCoreVertex);
	}
}