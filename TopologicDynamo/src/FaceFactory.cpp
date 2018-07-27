#include <FaceFactory.h>

#include <Face.h>

namespace Topologic
{
	Topology^ FaceFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	{
		TopologicCore::Face::Ptr pCoreFace = std::dynamic_pointer_cast<TopologicCore::Face>(kpTopology);
		if (pCoreFace == nullptr)
		{
			return nullptr;
		}
		return gcnew Face(pCoreFace);
	}
}