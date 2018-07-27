#include <FaceContentFactory.h>

#include <Face.h>

namespace Topologic
{
	Topology^ FaceContentFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	{
		TopologicCore::Face::Ptr pCoreFace = std::dynamic_pointer_cast<TopologicCore::Face>(kpTopology);
		if (pCoreFace == nullptr)
		{
			return nullptr;
		}
		return gcnew Face(pCoreFace);
	}
}