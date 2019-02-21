#include <LoadFactory.h>
#include <Load.h>

#include <TopologicCore/include/Vertex.h>

namespace TopologicStructure
{
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