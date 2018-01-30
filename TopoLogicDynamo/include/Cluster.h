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

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	protected:
		/// <summary>
		/// 
		/// </summary>
		Cluster();
		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Cluster* m_pCoreCluster;
	};
}