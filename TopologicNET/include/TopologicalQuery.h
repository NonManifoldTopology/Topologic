#pragma once

#include <TopologicCore/include/TopologicalQuery.h>

namespace Topologic
{
	/// <summary>
	/// TopologicalQuery is the base class for Topology and Context classes. 
	/// </summary>

	public ref class TopologicalQuery abstract
	{
	public protected:
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() = 0;

	protected:
		TopologicalQuery() {}
		virtual ~TopologicalQuery() {}
	};
}