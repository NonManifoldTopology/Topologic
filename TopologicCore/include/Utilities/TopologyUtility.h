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

#include <Topology.h>
#include <Cell.h>
#include <Face.h>
#include <Edge.h>
#include <Vertex.h>

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
		/// <param name="x"></param>
		/// <param name="y"></param>
		/// <param name="z"></param>
		/// <returns></returns>
		static TOPOLOGIC_API TopologicCore::Topology::Ptr Translate(
			const TopologicCore::Topology::Ptr& kpTopology, const double x = 0.0, const double y = 0.0, const double z = 0.0);

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
		static TOPOLOGIC_API TopologicCore::Topology::Ptr Rotate(const TopologicCore::Topology::Ptr& kpTopology,
			const TopologicCore::Vertex::Ptr& kpOrigin,
			const double kDirectionX = 0.0, const double kDirectionY = 0.0, const double kDirectionZ = 1.0,
			const double kDegree = 0.0);

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpOrigin"></param>
		/// <param name="kNormalX"></param>
		/// <param name="kNormalY"></param>
		/// <param name="kNormalZ"></param>
		/// <param name="kXAxisX"></param>
		/// <param name="kXAxisY"></param>
		/// <param name="kXAxisZ"></param>
		/// <returns></returns>
		static TOPOLOGIC_API TopologicCore::Topology::Ptr Transform(const TopologicCore::Topology::Ptr& kpTopology,
			const TopologicCore::Vertex::Ptr& kpOrigin,
			const double kNormalX, const double kNormalY, const double kNormalZ,
			const double kXAxisX, const double kXAxisY, const double kXAxisZ);*/

		static TOPOLOGIC_API TopologicCore::Topology::Ptr Transform(const TopologicCore::Topology::Ptr& kpTopology,
			const double kTranslationX = 0.0, const double kTranslationY = 0.0, const double kTranslationZ = 0.0,
			const double kRotation11 = 1.0, const double kRotation12 = 0.0, const double kRotation13 = 0.0,
			const double kRotation21 = 0.0, const double kRotation22 = 1.0, const double kRotation23 = 0.0,
			const double kRotation31 = 0.0, const double kRotation32 = 0.0, const double kRotation33 = 1.0);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kpOrigin"></param>
		/// <param name="kXFactor"></param>
		/// <param name="kYFactor"></param>
		/// <param name="kZFactor"></param>
		/// <returns></returns>
		static TOPOLOGIC_API TopologicCore::Topology::Ptr Scale(
			const TopologicCore::Topology::Ptr& kpTopology, const TopologicCore::Vertex::Ptr& kpOrigin, 
			const double kXFactor = 1.0, const double kYFactor = 1.0, const double kZFactor = 1.0);

		static TOPOLOGIC_API double DegreeToRadian(const double kDegree);

		static TOPOLOGIC_API double RadianToDegree(const double kRadian);

		static TOPOLOGIC_API void AdjacentTopologies(
			const TopologicCore::Topology::Ptr& kpCoreTopology, 
			const TopologicCore::Topology::Ptr& kpCoreParentTopology, 
			const int kTypeFilter,
			std::list<TopologicCore::Topology::Ptr>& rCoreAdjacentTopologies);
	};
}