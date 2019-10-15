// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

#pragma once

#include <Edge.h>
#include <Face.h>
#include <Shell.h>
#include <Vertex.h>
#include <Wire.h>

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
		static TOPOLOGIC_API TopologicCore::Shell::Ptr ByVerticesFaceIndices(const std::vector<TopologicCore::Vertex::Ptr>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_API TopologicCore::Shell::Ptr ByLoft(const std::list<TopologicCore::Wire::Ptr>& rkWires);

		/*/// <summary>
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
		static TOPOLOGIC_API TopologicCore::Shell::Ptr ByFacePlanarization(
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
		static TOPOLOGIC_API TopologicCore::Shell::Ptr ByFacePlanarization(
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


		static TOPOLOGIC_API TopologicCore::Shell::Ptr ByFacePlanarizationV2(
			const TopologicCore::Face::Ptr& kpFace,
			const int kIteration,
			const int kEdgeSamples,
			const std::list<double>& rkUValues,
			const std::list<double>& rkVValues
		);*/

	};
}