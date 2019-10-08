#pragma once

#include <Edge.h>
#include <Vertex.h>
#include <Wire.h>

#include <memory>

namespace TopologicUtilities
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
		static TOPOLOGIC_API TopologicCore::Edge::Ptr ByVertices(const std::list<TopologicCore::Vertex::Ptr>& rkVertices);

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
		static TOPOLOGIC_API TopologicCore::Edge::Ptr ByCircle(
			const TopologicCore::Vertex::Ptr& kpCenterPoint, const double kRadius,
			const double kXAxisX, const double kXAxisY, const double kXAxisZ,
			const double kNormalX, const double kNormalY, const double kNormalZ);
		
		static TOPOLOGIC_API TopologicCore::Edge::Ptr ByEllipse(
			const std::shared_ptr<TopologicCore::Vertex>& kpCenterPoint, const double kMajorRadius, const double kMinorRadius,
			const double kXAxisX, const double kXAxisY, const double kXAxisZ,
			const double kNormalX, const double kNormalY, const double kNormalZ);

		static TOPOLOGIC_API TopologicCore::Edge::Ptr ByNurbsCurve(
			const std::list<TopologicCore::Vertex::Ptr>& rkControlPoints,
			const std::list<double>& rkKnots,
			const std::list<double>& rkWeights,
			const int kDegree,
			const bool kIsPeriodic,
			const bool kIsRational);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpEdge"></param>
		/// <param name="kpVertex"></param>
		/// <returns></returns>
		static TOPOLOGIC_API double ParameterAtPoint(const TopologicCore::Edge::Ptr& kpEdge, const TopologicCore::Vertex::Ptr& kpVertex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpEdge"></param>
		/// <param name="kParameter"></param>
		/// <returns></returns>
		static TOPOLOGIC_API TopologicCore::Vertex::Ptr PointAtParameter(const TopologicCore::Edge::Ptr& kpEdge, const double kParameter);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpEdge"></param>
		/// <returns></returns>
		static TOPOLOGIC_API double Length(const TopologicCore::Edge::Ptr& kpEdge);

		static TOPOLOGIC_API void AdjacentWires(
			const TopologicCore::Edge::Ptr & kpEdge,
			const TopologicCore::Topology::Ptr & kpParentTopology,
			std::list<TopologicCore::Wire::Ptr>& rCoreAdjacentWires);

		static double AngleBetween(
			const TopologicCore::Edge::Ptr & kpEdge1,
			const TopologicCore::Edge::Ptr & kpEdge2
		);
	};
}