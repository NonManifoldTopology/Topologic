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
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Topology::Ptr Translate(const TopologicCore::Topology::Ptr& kpTopology, const double x, const double y, const double z);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpOrigin"></param>
		/// <param name="kDirectionX"></param>
		/// <param name="kDirectionY"></param>
		/// <param name="kDirectionZ"></param>
		/// <param name="kDegree"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Topology::Ptr Rotate(const TopologicCore::Topology::Ptr& kpTopology,
			const TopologicCore::Vertex::Ptr& kpOrigin,
			const double kDirectionX, const double kDirectionY, const double kDirectionZ,
			const double kDegree);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpOrigin"></param>
		/// <param name="kXFactor"></param>
		/// <param name="kYFactor"></param>
		/// <param name="kZFactor"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Topology::Ptr Scale(
			const TopologicCore::Topology::Ptr& kpTopology, const TopologicCore::Vertex::Ptr& kpOrigin, 
			const double kXFactor, const double kYFactor, const double kZFactor);

		static TOPOLOGIC_UTILITIES_API double DegreeToRadian(const double kDegree);

		static TOPOLOGIC_UTILITIES_API double RadianToDegree(const double kRadian);
	};
}