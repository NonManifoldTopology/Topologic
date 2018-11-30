#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Face.h>
#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Wire.h>

#include <memory>

namespace TopologicUtility
{
	class FaceUtility
	{
	public:
		typedef std::shared_ptr<FaceUtility> Ptr;

	public:

		/// <summary>
		/// Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kU"></param>
		/// <param name="kV"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API gp_Dir NormalAtParameter(const TopologicCore::Face::Ptr& kpFace, const double kU, const double kV);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API double Area(const TopologicCore::Face::Ptr& kpFace);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="coreVertices"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Face::Ptr ByVertices(const std::list<std::list<TopologicCore::Vertex::Ptr>>& rkVertices);

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kpVertex"></param>
		/// <param name="rU"></param>
		/// <param name="rV"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API void UVParameterAtVertex(const TopologicCore::Face::Ptr& kpFace, const TopologicCore::Vertex::Ptr& kpVertex, double& rU, double &rV);

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kU"></param>
		/// <param name="kV"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Vertex::Ptr VertexAtParameter(const TopologicCore::Face::Ptr& kpFace, const double kU, const double kV);

		static void UVSamplePoints(
			const TopologicCore::Face::Ptr& kpFace,
			const std::list<double>& rkUValues,
			const std::list<double>& rkVValues,
			std::list<std::list<gp_Pnt>>& rSamplePoints,
			std::list<double>& rOcctUValues,
			std::list<double>& rOcctVValues,
			int& rNumUPoints,
			int& rNumVPoints,
			int& rNumUPanels,
			int& rNumVPanels,
			bool& rIsUClosed,
			bool& rIsVClosed);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kpWire"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Face::Ptr TrimByWire(const TopologicCore::Face::Ptr& kpFace, const TopologicCore::Wire::Ptr& kpWire);

		static TOPOLOGIC_SUPPORT_API void Triangulate(const TopologicCore::Face::Ptr& kpFace, const double kDeflection, std::list<TopologicCore::Face::Ptr>& rTriangles);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kNonNormalizedU"></param>
		/// <param name="kNonNormalizedV"></param>
		/// <param name="rNormalizedU"></param>
		/// <param name="rNormalizedV"></param>
		static void NormalizeUV(const TopologicCore::Face::Ptr& kpFace, const double kNonNormalizedU, const double kNonNormalizedV, double& rNormalizedU, double& rNormalizedV);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kNormalizedU"></param>
		/// <param name="kNormalizedV"></param>
		/// <param name="rNonNormalizedU"></param>
		/// <param name="rNonNormalizedV"></param>
		static void NonNormalizeUV(const TopologicCore::Face::Ptr& kpFace, const double kNormalizedU, const double kNormalizedV, double& rNonNormalizedU, double& rNonNormalizedV);

	};
}