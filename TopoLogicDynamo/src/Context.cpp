#include <Context.h>

#include <assert.h>

namespace TopoLogic
{
	Context^ Context::ByTopologyParameters(TopoLogic::Topology^ topology, double U, double V, double W)
	{
		TopoLogicCore::Context* pCoreContext = TopoLogicCore::Context::ByTopologyParameters(
			TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Topology>(topology->GetCoreTopologicalQuery()),
			U, V, W);
		return gcnew Context(pCoreContext);
	}

	Topology^ Context::Topology()
	{
		TopoLogicCore::Context* pCoreContext = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(GetCoreTopologicalQuery());
		return Topology::ByCoreTopology(pCoreContext->Topology());
	}

	double Context::U()
	{
		TopoLogicCore::Context* pCoreContext = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(GetCoreTopologicalQuery());
		return pCoreContext->U();
	}

	double Context::V()
	{
		TopoLogicCore::Context* pCoreContext = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(GetCoreTopologicalQuery());
		return pCoreContext->V();
	}

	double Context::W()
	{
		TopoLogicCore::Context* pCoreContext = TopoLogicCore::TopologicalQuery::Downcast<TopoLogicCore::Context>(GetCoreTopologicalQuery());
		return pCoreContext->W();
	}

	Context::Context(TopoLogicCore::Context * const kpCoreContext)
		: m_pCoreContext(kpCoreContext)
	{
	}

	Context::~Context()
	{
		delete m_pCoreContext;
	}

	TopoLogicCore::TopologicalQuery* Context::GetCoreTopologicalQuery()
	{
		assert(m_pCoreContext != nullptr && "Context::GetCoreTopologicalQuery() returns null.");
		if (m_pCoreContext == nullptr)
		{
			throw gcnew Exception("Context::GetCoreTopologicalQuery() returns null.");
		}

		return m_pCoreContext;
	}
}