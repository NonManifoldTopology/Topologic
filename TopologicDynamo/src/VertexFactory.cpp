#include <VertexFactory.h>

#include <Vertex.h>

namespace Topologic
{
	Topology ^ VertexFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = std::dynamic_pointer_cast<TopologicCore::Vertex>(kpTopology.topologyPtr);
		if (pCoreVertex == nullptr)
		{
			return nullptr;
		}
		return gcnew Vertex(pCoreVertex);
	}
}