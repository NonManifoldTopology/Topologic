#pragma once

#include "Utilities.h"
#include "Topology.h"

#include <list>
#include <memory>

namespace TopoLogicCore
{
	class Context;

	class TopologyPairKey
	{
	public:
		TopologyPairKey(const TopoDS_Shape& rkOcctShape1, const TopoDS_Shape& rkOcctShape2)
			: m_rkOcctShape1(rkOcctShape1)
			, m_rkOcctShape2(rkOcctShape2)
		{

		}

		~TopologyPairKey()
		{

		}

		bool operator < (const TopologyPairKey& rkAnotherKey) const 
		{ 
			return this < &rkAnotherKey;
		}

		const TopoDS_Shape& GetTopology1() const { return m_rkOcctShape1; }
		const TopoDS_Shape& GetTopology2() const { return m_rkOcctShape2; }

	protected:
		const TopoDS_Shape& m_rkOcctShape1;
		const TopoDS_Shape& m_rkOcctShape2;
	};

	struct ApertureStatus
	{
		bool isOpen;
	};

	/// <summary>
	/// <para>
	/// An Aperture defines a connection path between two Topologies that share a common Topology or between a
	/// Topology and the outside space. A connection path can be uni-directional or bi-directional.
	/// </para>
	/// <para>An aperture can be:</para>
	/// <para>- An edge: for adjacent faces</para>
	/// <para>- A face: for adjacent cells or cell complexes</para>
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
		Topology* Topology() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API bool IsOpen() const;

		/// <summary>
		/// Checks if the topologies are adjacent.
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool IsOpen(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies) const;

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
		TOPOLOGIC_API void Open(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		TOPOLOGIC_API void Close();
		
		/// <summary>
		/// Checks if the topologies are adjacent.
		/// </summary>
		/// <param name="rkTopologies"></param>
		TOPOLOGIC_API void Close(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies);

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

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopologyType GetType() const;

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		void CheckQueryInputValidity(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::map<TopologyPairKey, ApertureStatus>::const_iterator FindStatus(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies, const bool kRaiseExceptionIfNotFound) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		std::map<TopologyPairKey, ApertureStatus>::iterator FindStatus(const std::array<TopoLogicCore::Topology*, 2>& rkTopologies, const bool kRaiseExceptionIfNotFound);

		void InitialisePairwiseStatuses(const bool kOpenStatus);

		std::map<TopologyPairKey, ApertureStatus> m_apertureStatuses;
		TopoLogicCore::Topology* m_pTopology;
	};
}