#include <Aperture.h>

namespace TopoLogicCore
{
	Aperture* Aperture::ByTopologyContext(TopoLogicCore::Topology* const kpTopology, Context* const kpContext)
	{
		return new Aperture(kpTopology, kpContext);
	}

	bool Aperture::IsOpen() const
	{
		return false;
	}

	void Aperture::FirstPath(std::list<TopoLogicCore::Topology*>& rFirstPath) const
	{
	}

	void Aperture::SecondPath(std::list<TopoLogicCore::Topology*>& rSecondPath) const
	{
	}

	Aperture::Aperture(TopoLogicCore::Topology* const kpTopology, Context* const kpContext)
	{
	}

	Aperture::~Aperture()
	{
	}
}