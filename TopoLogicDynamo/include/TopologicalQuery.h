#pragma once

#include <TopoLogicCore/include/TopologicalQuery.h>

namespace TopoLogic
{
	public ref class TopologicalQuery abstract
	{
	public protected:
		virtual std::shared_ptr<TopoLogicCore::TopologicalQuery> GetCoreTopologicalQuery() = 0;

	protected:
		TopologicalQuery() {}
		virtual ~TopologicalQuery() {}
	};
}