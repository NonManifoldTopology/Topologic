#pragma once

#include <TopologicCore/include/TopologicalQuery.h>

namespace Topologic
{
	public ref class TopologicalQuery abstract
	{
	public protected:
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() = 0;

	protected:
		TopologicalQuery() {}
		virtual ~TopologicalQuery() {}
	};
}