#pragma once

namespace TopoLogicCore
{
	/// <summary>
	/// The root class in the TopoLogic library
	/// </summary>
	class TopologicalQuery
	{
	public:
		template <typename T>
		static T* Downcast(TopologicalQuery *const kpTopologicalQuery)
		{
			T* pSubclassTopology = dynamic_cast<T*>(kpTopologicalQuery);
			if (pSubclassTopology == nullptr)
			{
				throw std::exception("Failed downcasting a topological query");
			}
			return pSubclassTopology;
		}

		template <typename T>
		static T const * Downcast(TopologicalQuery const *const kpkTopologicalQuery)
		{
			T const * kpSubclassTopology = dynamic_cast<T const *>(kpkTopologicalQuery);
			if (kpSubclassTopology == nullptr)
			{
				throw std::exception("Failed downcasting topology");
			}
			return kpSubclassTopology;
		}

		virtual ~TopologicalQuery() {};

	};
}