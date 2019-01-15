#pragma once

#include <Cluster.h>

#include <TopologicExtensions/include/Graph.h>

namespace Topologic
{
	ref class CellComplex;

	namespace Extensions
	{
		public ref class DualGraph_ : Cluster
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
				[DefaultArgument("true")] bool useCells,
				[DefaultArgument("true")] bool useNonManifoldFaces,
				[DefaultArgument("false")] bool useManifoldFaces,
				[DefaultArgument("false")] bool useApertures);


			/*property Object^ Geometry_
			{
				/// <summary>
				/// 
				/// </summary>
				/// <returns></returns>
				virtual Object^ get() override;
			}*/

		public protected:
			/// <summary>
			/// 
			/// </summary>
			/// <param name="kpCoreCluster"></param>
			DualGraph_(const std::shared_ptr<TopologicCore::Cluster>& kpCoreCluster);

			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

		protected:
			virtual ~DualGraph_();

		};
	}
}