#include <Context.h>

namespace TopologicStructure 
{
	Context^ Context::Create(Topology^ hostTopology, double u, double v, double w, double uScale, double vScale, double wScale, double uShift, double vShift, double wShift)
	{
		return gcnew Context(hostTopology, u, v, w, uScale, vScale, wScale, uShift, vShift, wShift);
	}

	Context::Context(Topology ^ hostTopology, double u, double v, double w, double uScale, double vScale, double wScale, double uShift, double vShift, double wShift)
		: m_pHostTopology(hostTopology)
		, m_u(u)
		, m_v(v)
		, m_w(w)
		, m_uScale(uScale)
		, m_vScale(vScale)
		, m_wScale(wScale)
		, m_uShift(uShift)
		, m_vShift(vShift)
		, m_wShift(wShift)
	{

	}

	Context::~Context()
	{

	}
}