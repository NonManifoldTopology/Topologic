#include <Context.h>

#include <assert.h>

namespace Topologic
{
	Context^ Context::ByTopologyParameters__(Topologic::Topology^ topology, double U, double V, double W)
	{
		std::shared_ptr<TopologicCore::Context> pCoreContext = TopologicCore::Context::ByTopologyParameters(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()),
			U, V, W);
		return gcnew Context(pCoreContext);
	}

	Topology^ Context::Topology::get()
	{
		std::shared_ptr<TopologicCore::Context> pCoreContext = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(GetCoreTopologicalQuery());
		return Topologic::Topology::ByCoreTopology(pCoreContext->Topology());
	}

	double Context::U()
	{
		std::shared_ptr<TopologicCore::Context> pCoreContext = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(GetCoreTopologicalQuery());
		return pCoreContext->U();
	}

	double Context::V()
	{
		std::shared_ptr<TopologicCore::Context> pCoreContext = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(GetCoreTopologicalQuery());
		return pCoreContext->V();
	}

	double Context::W()
	{
		std::shared_ptr<TopologicCore::Context> pCoreContext = TopologicCore::TopologicalQuery::Downcast<TopologicCore::Context>(GetCoreTopologicalQuery());
		return pCoreContext->W();
	}

	Context::Context(const std::shared_ptr<TopologicCore::Context>& kpCoreContext)
		: m_pCoreContext(new std::shared_ptr<TopologicCore::Context>(kpCoreContext))
	{
	}

	Context::~Context()
	{
		//delete m_pCoreContext;
	}

	std::shared_ptr<TopologicCore::TopologicalQuery> Context::GetCoreTopologicalQuery()
	{
		assert(m_pCoreContext != nullptr && "Context::GetCoreTopologicalQuery() returns null.");
		if (m_pCoreContext == nullptr)
		{
			throw gcnew Exception("Context::GetCoreTopologicalQuery() returns null.");
		}

		return *m_pCoreContext;
	}
}