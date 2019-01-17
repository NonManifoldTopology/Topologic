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
			/// <param name="direct"></param>
			/// <param name="viaSharedFaces"></param>
			/// <param name="viaSharedApertures"></param>
			/// <param name="toExteriorFaces"></param>
			/// <param name="toExteriorApertures"></param>
			/// <returns></returns>
			static DualGraph_^ ByCellComplex(
				CellComplex^ cellComplex,
				[DefaultArgument("true")] bool direct,
				[DefaultArgument("false")] bool viaSharedFaces,
				[DefaultArgument("false")] bool viaSharedApertures,
				[DefaultArgument("false")] bool toExteriorFaces,
				[DefaultArgument("false")] bool toExteriorApertures);

			/// <summary>
			/// 
			/// </summary>
			/// <param name="cellComplex"></param>
			/// <param name="direct"></param>
			/// <param name="viaSharedEdges"></param>
			/// <param name="viaSharedApertures"></param>
			/// <param name="toExteriorEdges"></param>
			/// <param name="toExteriorApertures"></param>
			/// <returns></returns>
			static DualGraph_^ ByShell(
				Shell^ shell,
				[DefaultArgument("true")] bool direct,
				[DefaultArgument("false")] bool viaSharedEdges,
				[DefaultArgument("false")] bool viaSharedApertures,
				[DefaultArgument("false")] bool toExteriorEdges,
				[DefaultArgument("false")] bool toExteriorApertures);

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