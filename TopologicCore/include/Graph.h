#pragma once

//#include "Utilities.h"
#include "Wire.h"

namespace TopologicCore
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
	/// <para>
	/// A Graph is a Wire that is defined by the topology of a CellComplex or a Shell. It can be manifold or non-manifold.
	/// </para>
	/// </summary>
	class Graph : public Wire
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

		static TOPOLOGIC_API Graph::Ptr ByCellComplex(
			const std::shared_ptr<CellComplex>& kpCellComplex,
			const bool kUseCells,
			const bool kUseNonManifoldFaces,
			const bool kUseManifoldFaces,
			const bool kUseApertures);

		virtual TopologyType GetType() const { return TOPOLOGY_GRAPH; }

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