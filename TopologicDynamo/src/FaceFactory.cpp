#include <FaceFactory.h>

#include <Face.h>

namespace Topologic
{
	//Topology^ FaceFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	//{
	//	TopologicCore::Face::Ptr pCoreFace = std::dynamic_pointer_cast<TopologicCore::Face>(kpTopology);
	//	if (pCoreFace == nullptr)
	//	{
	//		return nullptr;
	//	}
	//	return gcnew Face(pCoreFace);
	//}
	//Topology ^ FaceFactory::Create(TopologicCore::Topology const * const kpTopology)
	//{
	//	throw gcnew System::NotImplementedException();
	//	// TODO: insert return statement here
	//}
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