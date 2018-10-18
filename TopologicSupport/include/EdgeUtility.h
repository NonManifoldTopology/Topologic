#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Edge.h>
#include <TopologicCore/include/Vertex.h>

#include <memory>

namespace TopologicSupport
{
	class EdgeUtility
	{
	public:
		typedef std::shared_ptr<EdgeUtility> Ptr;

	public:

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Edge::Ptr ByVertices(const std::list<TopologicCore::Vertex::Ptr>& rkVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCenterPoint"></param>
		/// <param name="kRadius"></param>
		/// <param name="kXAxisX"></param>
		/// <param name="kXAxisY"></param>
		/// <param name="kXAxisZ"></param>
		/// <param name="kNormalX"></param>
		/// <param name="kNormalY"></param>
		/// <param name="kNormalZ"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Edge::Ptr ByCircle(
			const TopologicCore::Vertex::Ptr& kpCenterPoint, const double kRadius,
			const double kXAxisX, const double kXAxisY, const double kXAxisZ,
			const double kNormalX, const double kNormalY, const double kNormalZ);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpEdge"></param>
		/// <param name="kpVertex"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API double ParameterAtPoint(const TopologicCore::Edge::Ptr& kpEdge, const TopologicCore::Vertex::Ptr& kpVertex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpEdge"></param>
		/// <param name="kParameter"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Vertex::Ptr PointAtParameter(const TopologicCore::Edge::Ptr& kpEdge, const double kParameter);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpEdge"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API double Length(const TopologicCore::Edge::Ptr& kpEdge);
	};
}