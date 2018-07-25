#pragma once

#include <memory>

namespace TopologicCore
{
	/// <summary>
	/// The root class in the Topologic library
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
				throw std::exception("Failed downcasting a Topological query");
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

		template <typename T>
		static std::shared_ptr<T> Downcast(std::shared_ptr<TopologicalQuery>& rTopologicalQuery)
		{
			std::shared_ptr<T> pSubclassTopology = std::dynamic_pointer_cast<T>(kpTopologicalQuery);
			if (pSubclassTopology == nullptr)
			{
				throw std::exception("Failed downcasting a Topological query");
			}
			return pSubclassTopology;
		}

		template <typename T>
		static const std::shared_ptr<T> Downcast(const std::shared_ptr<TopologicalQuery>& rkTopologicalQuery)
		{
			const std::shared_ptr<T> kSubclassTopology = std::dynamic_pointer_cast<T>(rkTopologicalQuery);
			if (kSubclassTopology == nullptr)
			{
				throw std::exception("Failed downcasting a Topological query");
			}
			return kSubclassTopology;
		}

		template <typename T>
		static std::shared_ptr<T> Upcast(std::shared_ptr<TopologicalQuery>& rTopologicalQuery)
		{
			std::shared_ptr<T> pSuperClassTopology = std::dynamic_pointer_cast<T>(kpTopologicalQuery);
			if (pSuperClassTopology == nullptr)
			{
				throw std::exception("Failed upcasting a Topological query");
			}
			return pSuperClassTopology;
		}

		template <typename T>
		static const std::shared_ptr<T> Upcast(const std::shared_ptr<TopologicalQuery>& rkTopologicalQuery)
		{
			const std::shared_ptr<T> pSuperClassTopology = std::dynamic_pointer_cast<T>(rkTopologicalQuery);
			if (pSuperClassTopology == nullptr)
			{
				throw std::exception("Failed upcasting a Topological query");
			}
			return pSuperClassTopology;
		}

		virtual ~TopologicalQuery() {};

	};
}