// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <Context.h>

#include <assert.h>

namespace Topologic
{
	/*Context^ Context::ByTopologyParameters__(Topologic::Topology^ topology, double U, double V, double W)
	{
		std::shared_ptr<TopologicCore::Context> pCoreContext = TopologicCore::Context::ByTopologyParameters(
			TopologicCore::TopologicalQuery::Downcast<TopologicCore::Topology>(topology->GetCoreTopologicalQuery()),
			U, V, W);
		return gcnew Context(pCoreContext);
	}*/

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

	Context::Context()
		: Topologic::TopologicalQuery()
		, m_pCoreContext(nullptr)
	{

	}

	Context::Context(const std::shared_ptr<TopologicCore::Context>& kpCoreContext)
		: m_pCoreContext(new std::shared_ptr<TopologicCore::Context>(kpCoreContext))
	{
	}

	Context::~Context()
	{
		delete m_pCoreContext;
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