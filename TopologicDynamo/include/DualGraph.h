#pragma once

#include <Wire.h>

#include <TopologicCore/include/DualGraph.h>

namespace Topologic
{
	ref class CellComplex;

	public ref class DualGraph : Wire
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cellComplex"></param>
		/// <returns></returns>
		static DualGraph^ ByCellComplex(CellComplex^ cellComplex);


		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreWire"></param>
		DualGraph(const std::shared_ptr<TopologicCore::Wire>& kpCoreWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pEdges"></param>
		DualGraph(IEnumerable<Edge^>^ pEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoPolycurve"></param>
		DualGraph(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~DualGraph();

	};
}