#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Wire.h>

namespace TopologicExtension
{
	class GraphGUID
	{
	public:
		static std::string Get()
		{
			return std::string("e9ad9252-f758-4dcb-8b9a-0979f4fa8c87");
		}
	};

	/// <summary>
	/// 
	/// </summary>
	class Graph : public TopologicCore::Wire
	{
	public:
		typedef std::shared_ptr<Graph> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctWire"></param>
		Graph(const TopoDS_Wire& rkOcctWire, const std::string& rkGuid = "");

		virtual ~Graph();

		static TOPOLOGIC_EXTENSION_API Graph::Ptr ByCellComplex(
			const std::shared_ptr<TopologicCore::CellComplex>& kpCellComplex,
			const bool kUseCells,
			const bool kUseNonManifoldFaces,
			const bool kUseManifoldFaces,
			const bool kUseApertures);

		virtual TopologicCore::TopologyType GetType() const { return TopologicCore::TOPOLOGY_WIRE; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return GraphGUID::Get();
		}

	protected:
	};
}