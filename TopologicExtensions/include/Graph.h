#pragma once

#include "Utilities.h"

#include <TopologicCore/include/Cluster.h>

namespace TopologicExtensions
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
	class DualGraph_ : public TopologicCore::Cluster
	{
	public:
		typedef std::shared_ptr<DualGraph_> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctWire"></param>
		/// <param name="rkGuid"></param>
		DualGraph_(const TopoDS_Compound& rkOcctCompound, const std::string& rkGuid = "");

		virtual ~DualGraph_();

		static TOPOLOGIC_EXTENSIONS_API DualGraph_::Ptr ByCellComplex(
			const std::shared_ptr<TopologicCore::CellComplex>& kpCellComplex,
			const bool kDirect,
			const bool kViaSharedFaces,
			const bool kViaSharedApertures,
			const bool kToExteriorFaces,
			const bool kToExteriorApertures);

		static TOPOLOGIC_EXTENSIONS_API DualGraph_::Ptr ByShell(
			const std::shared_ptr<TopologicCore::Shell>& kpShell,
			const bool kDirect,
			const bool kViaSharedEdges,
			const bool kViaSharedApertures,
			const bool kToExteriorEdges,
			const bool kToExteriorApertures);

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