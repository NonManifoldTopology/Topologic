#include <FaceFactory.h>

#include <Face.h>

namespace Topologic
{
	namespace Factories
	{
		Topology ^ FaceFactory::Create(const TopologicCore::TopologyPtr & kpTopology)
		{
			TopologicCore::Face::Ptr pCoreFace = std::dynamic_pointer_cast<TopologicCore::Face>(kpTopology.topologyPtr);
			if (pCoreFace == nullptr)
			{
				return nullptr;
			}
			return gcnew Face(pCoreFace);
		}
	}
}