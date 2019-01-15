#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Cell.h>
#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Wire.h>

#include <memory>

namespace TopologicUtilities
{
	class CellUtility
	{
	public:
		typedef std::shared_ptr<CellUtility> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Cell::Ptr ByVerticesFaceIndices(const std::vector<TopologicCore::Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Cell::Ptr ByLoft(const std::list<TopologicCore::Wire::Ptr>& rkWires);

		//
		static TOPOLOGIC_UTILITIES_API TopologicCore::Cell::Ptr ByCuboid(
			const double kCentroidX, const double kCentroidY, const double kCentroidZ, 
			const double kXDimension, const double kYDimension, const double kZDimension);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kCenterX"></param>
		/// <param name="kCenterY"></param>
		/// <param name="kCenterZ"></param>
		/// <param name="kRadius"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Cell::Ptr BySphere(const double kCenterX, const double kCenterY, const double kCenterZ, const double kRadius);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kReferencePointX"></param>
		/// <param name="kReferencePointY"></param>
		/// <param name="kReferencePointZ"></param>
		/// <param name="kNormalX"></param>
		/// <param name="kNormalY"></param>
		/// <param name="kNormalZ"></param>
		/// <param name="kRadius"></param>
		/// <param name="kHeight"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Cell::Ptr ByCylinder(
			const double kReferencePointX, const double kReferencePointY, const double kReferencePointZ,
			const double kNormalX, const double kNormalY, const double kNormalZ,
			const double kRadius, const double kHeight);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kReferencePointX"></param>
		/// <param name="kReferencePointY"></param>
		/// <param name="kReferencePointZ"></param>
		/// <param name="kNormalX"></param>
		/// <param name="kNormalY"></param>
		/// <param name="kNormalZ"></param>
		/// <param name="kXDirectionX"></param>
		/// <param name="kXDirectionY"></param>
		/// <param name="kDirectionZ"></param>
		/// <param name="kRadius1"></param>
		/// <param name="kRadius2"></param>
		/// <param name="kHeight"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Cell::Ptr ByCone(
			const double kReferencePointX, const double kReferencePointY, const double kReferencePointZ,
			const double kNormalX, const double kNormalY, const double kNormalZ,
			const double kXDirectionX, const double kXDirectionY, const double kDirectionZ,
			const double kRadius1, const double kRadius2, const double kHeight);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCell"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API double Volume(const TopologicCore::Cell::Ptr& kpCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCell"></param>
		/// <param name="kpVertex"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API bool DoesContain(const TopologicCore::Cell::Ptr & kpCell, const TopologicCore::Vertex::Ptr& kpVertex);

		static TOPOLOGIC_UTILITIES_API void GetMinMax(const TopologicCore::Cell::Ptr & kpCell, double &rMinX, double &rMaxX, double &rMinY, double &rMaxY, double &rMinZ, double &rMaxZ);
	};
}