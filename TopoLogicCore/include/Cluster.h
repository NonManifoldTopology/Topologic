#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_Compound.hxx>

namespace TopoLogicCore
{
	class Cluster : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctCompound"></param>
		Cluster(TopoDS_Compound * const kpOcctCompound);

		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		static Cluster* ByTopology(const std::list<Topology*>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const { return m_pOcctCompound; }

	protected:
		/// <summary>
		/// The underlying OCCT compound.
		/// </summary>
		TopoDS_Compound* m_pOcctCompound;
	};
}
