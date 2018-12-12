#pragma once

#include <Wire.h>

#include <TopologicExtension/include/Graph.h>

namespace Topologic
{
	ref class CellComplex;

	namespace Extension
	{
		public ref class Graph : Wire
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
			static Graph^ ByCellComplex(
				CellComplex^ cellComplex,
				[DefaultArgument("true")] bool useCells,
				[DefaultArgument("true")] bool useNonManifoldFaces,
				[DefaultArgument("false")] bool useManifoldFaces,
				[DefaultArgument("false")] bool useApertures);


			property Object^ Geometry_
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
			Graph(const std::shared_ptr<TopologicCore::Wire>& kpCoreWire);

			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

		protected:
			virtual ~Graph();

		};
	}
}