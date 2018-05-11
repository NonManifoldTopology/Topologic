#pragma once

#include "Cluster.h"

#include <TDF_Data.hxx>

namespace TopologicCore
{
	class GlobalCluster
	{
	public:
		static GlobalCluster& GetInstance()
		{
			static GlobalCluster instance;
			return instance;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		bool Add(Topology * const kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		bool Remove(Topology* pTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::shared_ptr<Cluster> GetCluster() const;

	private:
		GlobalCluster();
		GlobalCluster(GlobalCluster const&); // Don't Implement.
		void operator=(GlobalCluster const&); // Don't implement

		~GlobalCluster();

		std::shared_ptr<Cluster> m_pCluster;
		Handle(TDF_Data) m_pOcctDocument;
	};
}