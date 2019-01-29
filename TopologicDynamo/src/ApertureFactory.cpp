#include <ApertureFactory.h>

#include <Aperture.h>

namespace Topologic
{
	namespace Factories
	{
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
}