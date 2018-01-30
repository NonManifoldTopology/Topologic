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
		void Add(Topology const * const kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
		void Remove(Topology* kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Cluster* GetCluster() const { return m_pCluster; }

	private:
		GlobalCluster();
		GlobalCluster(GlobalCluster const&); // Don't Implement.
		void operator=(GlobalCluster const&); // Don't implement

		~GlobalCluster();

		Cluster* m_pCluster;
	};
}