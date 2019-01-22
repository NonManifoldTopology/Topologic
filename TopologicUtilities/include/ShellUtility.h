#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Edge.h>
#include <TopologicCore/include/Face.h>
#include <TopologicCore/include/Shell.h>
#include <TopologicCore/include/Vertex.h>
#include <TopologicCore/include/Wire.h>

#include <memory>

namespace TopologicUtilities
{
	class ShellUtility
	{
	public:
		typedef std::shared_ptr<ShellUtility> Ptr;

	public:

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Shell::Ptr ByVerticesFaceIndices(const std::vector<TopologicCore::Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Shell::Ptr ByLoft(const std::list<TopologicCore::Wire::Ptr>& rkWires);

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kIteration"></param>
		/// <param name="kNumUPanels"></param>
		/// <param name="kNumVPanels"></param>
		/// <param name="kTolerance"></param>
		/// <param name="kCapBottom"></param>
		/// <param name="kCapTop"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Shell::Ptr ByFacePlanarization(
			const TopologicCore::Face::Ptr& kpFace,
			const int kIteration,
			const int kEdgeSamples,
			const int kNumUPanels,
			const int kNumVPanels,
			const double kTolerance,
			const bool kCapBottom,
			const bool kCapTop,
			std::list<TopologicCore::Vertex::Ptr>& vertices,
			std::list<TopologicCore::Edge::Ptr>& edges,
			std::list<TopologicCore::Wire::Ptr>& wires,
			std::list<TopologicCore::Face::Ptr>& faces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kIteration"></param>
		/// <param name="rkUValues"></param>
		/// <param name="rkVValues"></param>
		/// <param name="kTolerance"></param>
		/// <param name="kCapBottom"></param>
		/// <param name="kCapTop"></param>
		/// <returns></returns>
		static TOPOLOGIC_UTILITIES_API TopologicCore::Shell::Ptr ByFacePlanarization(
			const TopologicCore::Face::Ptr& kpFace,
			const int kIteration,
			const int kEdgeSamples,
			const std::list<double>& rkUValues,
			const std::list<double>& rkVValues,
			const double kTolerance,
			const bool kCapBottom,
			const bool kCapTop,
			std::list<TopologicCore::Vertex::Ptr>& vertices,
			std::list<TopologicCore::Edge::Ptr>& edges,
			std::list<TopologicCore::Wire::Ptr>& wires,
			std::list<TopologicCore::Face::Ptr>& faces);


		static TOPOLOGIC_UTILITIES_API TopologicCore::Shell::Ptr ByFacePlanarizationV2(
			const TopologicCore::Face::Ptr& kpFace,
			const int kIteration,
			const int kEdgeSamples,
			const std::list<double>& rkUValues,
			const std::list<double>& rkVValues
		);

	};
}