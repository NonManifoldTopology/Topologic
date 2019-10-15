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

#include <Vertex.h>
#include <Edge.h>

#include <memory>

namespace TopologicUtilities
{
	class VertexUtility
	{
	public:
		typedef std::shared_ptr<VertexUtility> Ptr;

	public:
		static TOPOLOGIC_API void AdjacentEdges(
			const TopologicCore::Vertex::Ptr & kpVertex, 
			const TopologicCore::Topology::Ptr & kpParentTopology,
			std::list<TopologicCore::Edge::Ptr>& rCoreAdjacentEdges);

		static TOPOLOGIC_API void AdjacentEdges(
			const TopologicCore::Vertex::Ptr & kpVertex,
			TopologicCore::Topology const * const kpkParentTopology,
			std::list<TopologicCore::Edge::Ptr>& rCoreAdjacentEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpVertex"></param>
		/// <param name="kpTopology"></param>
		/// <returns></returns>
		static TOPOLOGIC_API double Distance(
			const TopologicCore::Vertex::Ptr& kpVertex, 
			const TopologicCore::Topology::Ptr& kpTopology);

	};
}