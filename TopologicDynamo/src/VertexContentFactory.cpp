#include <VertexContentFactory.h>

#include <Vertex.h>

namespace Topologic
{
	Topology^ VertexContentFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	{
		TopologicCore::Vertex::Ptr pCoreVertex = std::dynamic_pointer_cast<TopologicCore::Vertex>(kpTopology);
		if (pCoreVertex == nullptr)
		{
			return nullptr;
		}
		return gcnew Vertex(pCoreVertex);
	}
}