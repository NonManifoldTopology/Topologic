#pragma once

#include "Utilities.h"

#include <memory>

#include <TopologicCore/include/Topology.h>

namespace TopologicUtility
{
	class TopologyUtility
	{
	public:
		typedef std::shared_ptr<TopologyUtility> Ptr;

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API double Distance(const TopologicCore::Topology::Ptr& kpTopology, const TopologicCore::Topology::Ptr& kpAnotherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		static TOPOLOGIC_SUPPORT_API void Translate(const TopologicCore::Topology::Ptr& kpTopology, const double x, const double y, const double z);

	};
}