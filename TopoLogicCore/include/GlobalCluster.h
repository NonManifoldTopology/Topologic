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
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		bool Add(Topology const * const kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		bool Remove(Topology* kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Cluster* GetCluster() const;

	private:
		GlobalCluster();
		GlobalCluster(GlobalCluster const&); // Don't Implement.
		void operator=(GlobalCluster const&); // Don't implement

		~GlobalCluster();

		Cluster* m_pCluster;
	};
}