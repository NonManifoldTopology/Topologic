#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Cell.h>
#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Wire.h>

#include <memory>

namespace TopologicUtility
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
		static TOPOLOGIC_SUPPORT_API TopologicCore::Cell::Ptr ByVerticesFaceIndices(const std::vector<TopologicCore::Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API TopologicCore::Cell::Ptr ByLoft(const std::list<TopologicCore::Wire::Ptr>& rkWires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCell"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API double Volume(const TopologicCore::Cell::Ptr& kpCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCell"></param>
		/// <param name="kpVertex"></param>
		/// <returns></returns>
		static TOPOLOGIC_SUPPORT_API bool DoesContain(const TopologicCore::Cell::Ptr & kpCell, const TopologicCore::Vertex::Ptr& kpVertex);
	};
}