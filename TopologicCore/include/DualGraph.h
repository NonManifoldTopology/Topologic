#pragma once

//#include "Utilities.h"
#include "Wire.h"

namespace TopologicCore
{
	/// <summary>
	/// <para>
	/// A DualGraph is a Wire that is defined by the topology of a CellComplex or a Shell. It can be manifold or non-manifold.
	/// </para>
	/// </summary>
	class DualGraph : public Wire
	{
	public:
		typedef std::shared_ptr<DualGraph> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctWire"></param>
		DualGraph(const TopoDS_Wire& rkOcctWire, const std::string& rkGuid = "");

		virtual ~DualGraph();

		static TOPOLOGIC_API DualGraph::Ptr ByCellComplex(const std::shared_ptr<CellComplex>& kpCellComplex);

		virtual TopologyType GetType() const { return TOPOLOGY_DUALGRAPH; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return std::string("e9ad9252-f758-4dcb-8b9a-0979f4fa8c87");
		}

	protected:
	};
}