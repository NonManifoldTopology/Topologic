#include <ApertureFactory.h>

#include <Aperture.h>

namespace Topologic
{
	Topology^ ApertureFactory::Create(const TopologicCore::Topology::Ptr& kpTopology)
	{
		TopologicCore::Aperture::Ptr pCoreAperture = std::dynamic_pointer_cast<TopologicCore::Aperture>(kpTopology);
		if (pCoreAperture == nullptr)
		{
			return nullptr;
		}
		return gcnew Aperture(pCoreAperture);
	}
}