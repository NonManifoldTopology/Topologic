#pragma once

#include <Wire.h>

#include <TopologicCore/include/DualGraph.h>

namespace Topologic
{
	ref class CellComplex;

	public ref class DualGraph_ : Wire
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cellComplex"></param>
		/// <param name="useCells"></param>
		/// <param name="useNonManifoldFaces"></param>
		/// <param name="useManifoldFaces"></param>
		/// <param name="useApertures"></param>
		/// <returns></returns>
		static DualGraph_^ ByCellComplex(
			CellComplex^ cellComplex,
			bool useCells,
			bool useNonManifoldFaces,
			bool useManifoldFaces,
			bool useApertures);


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
		DualGraph_(const std::shared_ptr<TopologicCore::Wire>& kpCoreWire);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~DualGraph_();

	};
}