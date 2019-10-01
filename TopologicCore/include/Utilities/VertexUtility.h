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