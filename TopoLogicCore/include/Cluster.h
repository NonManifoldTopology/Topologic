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
		/// <param name="kAddToGlobalCluster"></param>
		Cluster(TopoDS_Compound * const kpOcctCompound, const bool kAddToGlobalCluster = true);

		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Cluster* ByTopology(const std::list<Topology*>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCluster"></param>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool Add(Topology const * const kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool Remove(Topology const * const kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

	protected:
		/// <summary>
		/// The underlying OCCT compound.
		/// </summary>
		TopoDS_Compound* m_pOcctCompound;
	};
}
