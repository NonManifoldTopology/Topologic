#pragma once

#include <Cluster.h>

#include <TopologicExtensions/include/Graph.h>

namespace Topologic
{
	ref class CellComplex;

	namespace Extensions
	/// <summary>
	/// Extension currently includes the Graph class.
	/// </summary>

	{
		public ref class DualGraph_ : Cluster
		/// <summary>
		/// A Graph consists of a set of vertices and a set of edges that connect the vertices. It can be a Wire, Vertex, Edge or Cluster and is defined by the input Topology. It can be manifold or non-manifold.
		/// </summary>
		{
		public:
			/// <summary>
			/// Creates a dual graph by a CellComplex. Options to be selected for the creation of the dual graph is the use of cells, non-manifold faces, manifold faces and apertures.
			/// </summary>
			/// <param name="cellComplex">The cell complex to be used for the creation of the dual graph</param>
			/// <param name="direct"></param>
			/// <param name="viaSharedFaces"></param>
			/// <param name="viaSharedApertures"></param>
			/// <param name="toExteriorFaces"></param>
			/// <param name="toExteriorApertures"></param>
			/// <param name="useCells">Choose whether to use the cells of the cell complex or not</param>
			/// <param name="useNonManifoldFaces">Choose whether to use the non-manifold faces of the cell complex or not</param>
			/// <param name="useManifoldFaces">Choose whether to use the manifold faces of the cell complex or not</param>
			/// <param name="useApertures">Choose whether to use the apertures of the cell complex or not</param>
			/// <returns>The created dual graph</returns>
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