#include <Context.h>
#include <Topology.h>

namespace TopoLogicCore
{
	Context* Context::ByTopologyParameters(TopoLogicCore::Topology * const kpTopology, const double kU, const double kV, const double kW)
	{
		return new Context(kpTopology, kU, kV, kW);
	}

	Context::Context(TopoLogicCore::Topology * const kpTopology, const double kU, double kV, double kW)
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