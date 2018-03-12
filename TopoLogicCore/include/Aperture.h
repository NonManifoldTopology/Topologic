#pragma once

#include "Utilities.h"
#include "TopologicalQuery.h"

#include <list>
#include <memory>

namespace TopoLogicCore
{
	class Context;
	class Topology;

	/// <summary>
	/// <para>
	/// An aperture controls connection paths between two topologies that share a common topology or between a
	/// topology and the outside space.It can be uni - directional or bi - directional.For each path direction, 
	/// it may allow a varying normalised degree of weights(i.e.between 0 and 1). 
	/// A closed aperture has a path weight of 0 for both paths.A fully open aperture has a weight of 1.
	/// </para>
	/// </summary>
	class Aperture : public TopologicalQuery
	{
	public:
		Aperture(TopoLogicCore::Topology* const kpTopology, Context * const kpContext);
		~Aperture();

		/// <summary>
		/// Creates an aperture by a topology and a context.
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpContext"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Aperture* ByTopologyContext(TopoLogicCore::Topology* const kpTopology, Context * const kpContext);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API double FirstPathWeight() const { return m_firstPathWeight; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API double SecondPathWeight() const { return m_secondPathWeight; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Topology* Topology() const { return m_pTopology; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API bool IsOpen() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFirstPath"></param>
		TOPOLOGIC_API void FirstPath(std::list<TopoLogicCore::Topology*>& rFirstPath) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rSecondPath"></param>
		TOPOLOGIC_API void SecondPath(std::list<TopoLogicCore::Topology*>& rSecondPath) const;

	protected:
		TopoLogicCore::Topology* m_pTopology;
		double m_firstPathWeight;
		double m_secondPathWeight;
	};
}