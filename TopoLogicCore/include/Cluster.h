#pragma once

#include <Topology.h>

#include <list>

class TopoDS_Compound;

namespace TopoLogicCore
{
	class Cluster : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		static Cluster* ByTopology(const std::list<Topology*>& rkTopologies);

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctCompound"></param>
		Cluster(TopoDS_Compound * const kpOcctCompound);
		virtual ~Cluster();

		/// <summary>
		/// The underlying OCCT compound.
		/// </summary>
		TopoDS_Compound* m_pOcctCompound;
	};
}
