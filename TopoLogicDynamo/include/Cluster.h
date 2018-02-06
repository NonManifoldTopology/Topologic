#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Cluster.h>

namespace TopoLogic
{
	public ref class Cluster : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		static Cluster^ ByTopology(List<Topology^>^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cluster"></param>
		/// <param name="topology"></param>
		/// <returns></returns>
		static Cluster^ Add(Cluster^ cluster, Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cluster"></param>
		/// <param name="topology"></param>
		/// <returns></returns>
		static Cluster^ Remove(Cluster^ cluster, Topology^ topology);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreCell"></param>
		Cluster(TopoLogicCore::Cluster* const kpCoreCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::Topology* GetCoreTopology() override;


	protected:
		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Cluster* m_pCoreCluster;
	};
}