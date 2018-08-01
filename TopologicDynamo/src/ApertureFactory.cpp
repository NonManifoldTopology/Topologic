#include <ApertureFactory.h>

#include <Aperture.h>

namespace Topologic
{
	//Topology^ ApertureFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	//{
	//	TopologicCore::Aperture::Ptr pCoreAperture = std::dynamic_pointer_cast<TopologicCore::Aperture>(kpTopology);
	//	if (pCoreAperture == nullptr)
	//	{
	//		return nullptr;
	//	}
	//	return gcnew Aperture(pCoreAperture);
	//}

	//Topology ^ ApertureFactory::Create(TopologicCore::Topology const * const kpTopology)
	//{
	//	throw gcnew System::NotImplementedException();
	//	// TODO: insert return statement here
	//}
	Topology ^ ApertureFactory::Create(const TopologicCore::TopologyPtr& kpTopology)
	{
		TopologicCore::Aperture::Ptr pCoreAperture = std::dynamic_pointer_cast<TopologicCore::Aperture>(kpTopology.topologyPtr);
		if (pCoreAperture == nullptr)
		{
			return nullptr;
		}
		return gcnew Aperture(pCoreAperture);
	}
}