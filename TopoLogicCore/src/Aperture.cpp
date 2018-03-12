#include <Aperture.h>

namespace TopoLogicCore
{
	Aperture* Aperture::ByTopologyContext(TopoLogicCore::Topology* const kpTopology, Context* const kpContext)
	{
		// TODO: what is the default value of the open status?
		return new Aperture(kpTopology, kpContext, false);
	}

	Aperture* Aperture::ByTopologyContextStatus(TopoLogicCore::Topology * const kpTopology, Context * const kpContext, const bool kOpenStatus)
	{
		return new Aperture(kpTopology, kpContext, kOpenStatus);
	}

	bool Aperture::IsOpen() const
	{
		return false;
	}

	bool Aperture::IsOpen(const std::list<TopoLogicCore::Topology*>& rkTopologies) const
	{
		return false;
	}

	void Aperture::Paths(std::list<std::list<TopoLogicCore::Topology*>>& rPaths) const
	{
		
	}

	void Aperture::Open()
	{
	}

	void Aperture::Open(const std::list<TopoLogicCore::Topology*>& rkTopologies)
	{
	}

	void Aperture::Close()
	{
	}

	void Aperture::Close(const std::list<TopoLogicCore::Topology*>& rkTopologies)
	{
	}

	void Aperture::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		m_pTopology->Geometry(rOcctGeometries);
	}

	TopoDS_Shape* Aperture::GetOcctShape() const
	{
		return m_pTopology->GetOcctShape();
	}

	Aperture::Aperture(TopoLogicCore::Topology* const kpTopology, Context* const kpContext, const bool kOpenStatus)
		: Topology(kpTopology->Dimensionality())
		, m_pTopology(kpTopology)
	{
	}

	Aperture::~Aperture()
	{
		delete m_pTopology;
	}
}