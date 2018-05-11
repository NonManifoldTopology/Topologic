#include <Context.h>
#include <Topology.h>

namespace TopologicCore
{
	std::shared_ptr<Context> Context::ByTopologyParameters(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const double kU, const double kV, const double kW)
	{
		return std::make_shared<Context>(kpTopology, kU, kV, kW);
	}

	Context::Context(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const double kU, double kV, double kW)
		: m_pTopology(kpTopology)
		, m_u(kU)
		, m_v(kV)
		, m_w(kW)
	{
	}

	Context::~Context()
	{
	}
}