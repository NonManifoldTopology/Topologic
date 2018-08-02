#include <Context.h>
#include <Topology.h>

namespace TopologicCore
{
	std::shared_ptr<Context> Context::ByTopologyParameters(const Topology::Ptr& kpTopology, const double kU, const double kV, const double kW)
	{
		return std::make_shared<Context>(kpTopology, kU, kV, kW);
	}

	Context::Context(const Topology::Ptr& kpTopology, const double kU, double kV, double kW)
		: m_occtShape(kpTopology->GetOcctShape())
		, m_u(kU)
		, m_v(kV)
		, m_w(kW)
	{
	}

	Context::~Context()
	{
	}
	
	std::shared_ptr<Topology> Context::Topology() const
	{
		return Topology::ByOcctShape(m_occtShape);
	}
}