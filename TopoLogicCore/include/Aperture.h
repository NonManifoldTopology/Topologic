#pragma once

#include "Utilities.h"
#include "Topology.h"

#include <list>
#include <memory>

namespace TopoLogicCore
{
	class Context;

	/// <summary>
	/// <para>
	/// An Aperture defines a connection path between two Topologies that share a common Topology or between a
	/// Topology and the outside space. A connection path can be uni-directional or bi-directional.
	/// </para>
	/// </summary>
	class Aperture : public Topology
	{
	public:
		Aperture(TopoLogicCore::Topology* const kpTopology, Context * const kpContext, const bool kOpenStatus);
		~Aperture();

		/// <summary>
		/// Creates an aperture by a topology and a context.
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpContext"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Aperture* ByTopologyContext(TopoLogicCore::Topology* const kpTopology, Context * const kpContext);

		/// <summary>
		/// Creates an aperture by a topology and a context.
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpContext"></param>
		/// <param name="kOpenStatus"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Aperture* ByTopologyContextStatus(TopoLogicCore::Topology* const kpTopology, Context * const kpContext, const bool kOpenStatus);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API bool IsOpen() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool IsOpen(const std::list<TopoLogicCore::Topology*>& rkTopologies) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rPaths"></param>
		TOPOLOGIC_API void Paths(std::list<std::list<TopoLogicCore::Topology*>>& rPaths) const;

		/// <summary>
		/// 
		/// </summary>
		TOPOLOGIC_API void Open();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		TOPOLOGIC_API void Open(const std::list<TopoLogicCore::Topology*>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		TOPOLOGIC_API void Close();
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		TOPOLOGIC_API void Close(const std::list<TopoLogicCore::Topology*>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shape* GetOcctShape() const;

		virtual TopologyType GetType() const { return TOPOLOGY_VERTEX; }

	protected:
		Topology* m_pTopology;
	};
}