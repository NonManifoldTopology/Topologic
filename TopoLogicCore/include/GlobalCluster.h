#pragma once

#include "Cluster.h"

namespace TopoLogicCore
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
		bool Add(const std::shared_ptr<Topology>& kpTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		bool Remove(const std::shared_ptr<Topology>& kpTopology);

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
	};
}