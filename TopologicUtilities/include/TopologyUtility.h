#pragma once

#include "Utilities.h"

#include <memory>

#include <TopologicCore/include/Topology.h>
#include <TopologicCore/include/Vertex.h>

namespace TopologicUtilities
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
		static TOPOLOGIC_UTILITIES_API double Distance(const TopologicCore::Topology::Ptr& kpTopology, const TopologicCore::Topology::Ptr& kpAnotherTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		static TOPOLOGIC_UTILITIES_API void Translate(const TopologicCore::Topology::Ptr& kpTopology, const double x, const double y, const double z);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kPointX"></param>
		/// <param name="kPointY"></param>
		/// <param name="kPointZ"></param>
		/// <param name="kPkDirectionXointX"></param>
		/// <param name="kDirectionY"></param>
		/// <param name="kDirectionZ"></param>
		/// <param name="kAngleInRadian"></param>
		static TOPOLOGIC_UTILITIES_API void Rotate(const TopologicCore::Topology::Ptr& kpTopology, 
			const double kPointX, const double kPointY, const double kPointZ, 
			const double kDirectionX, const double kDirectionY, const double kDirectionZ,
			const double kAngleInRadian);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpVertex"></param>
		/// <param name="kScaleFactor"></param>
		static TOPOLOGIC_UTILITIES_API void Scale(const TopologicCore::Topology::Ptr& kpTopology, const TopologicCore::Vertex::Ptr& kpVertex, const double kScaleFactor);

	};
}