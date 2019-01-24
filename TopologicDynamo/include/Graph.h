#pragma once

#include <Cluster.h>

#include <TopologicExtensions/include/Graph.h>

namespace Topologic
{
	ref class CellComplex;

	/// <summary>
	/// Extension currently includes the DualGraph class.
	/// </summary>
	namespace Extensions
	{
		/// <summary>
		/// A Graph consists of a set of vertices and a set of edges that connect the vertices. It can be a Wire, Vertex, Edge or Cluster and is defined by the input Topology. It can be manifold or non-manifold.
		/// </summary>
		public ref class DualGraph_ : Cluster
		
		{
		public:
			/// <summary>
			/// Creates a dual graph by a CellComplex.
			/// </summary>
			/// <param name="cellComplex">The CellComplex to be used for the creation of the dual graph</param>
			/// <param name="direct">Choose whether to use the centroids of the Cells in the CellComplex or not</param>
			/// <param name="viaSharedFaces">Choose whether to use the shared Faces in the CellComplex or not</param>
			/// <param name="viaSharedApertures">Choose whether to use any shared Apertures in the CellComplex or not</param>
			/// <param name="toExteriorFaces">Choose whether to use the exterior Faces of the CellComplex or not</param>
			/// <param name="toExteriorApertures">Choose whether to use any exterior Apertures of the CellComplex or not</param>
			/// <returns>The created dual graph</returns>
			static DualGraph_^ ByCellComplex(
				CellComplex^ cellComplex,
				[DefaultArgument("true")] bool direct,
				[DefaultArgument("false")] bool viaSharedFaces,
				[DefaultArgument("false")] bool viaSharedApertures,
				[DefaultArgument("false")] bool toExteriorFaces,
				[DefaultArgument("false")] bool toExteriorApertures);

			/// <summary>
			/// Creates a dual graph by a Shell.
			/// </summary>
			/// <param name="shell">The Shell to be used for the creation of the dual graph</param>
			/// <param name="direct">Choose whether to use the centroids of the Faces in the Shell or not</param>
			/// <param name="viaSharedEdges">Choose whether to use the shared Edges in the Shell or not</param>
			/// <param name="viaSharedApertures">Choose whether to use any shared Apertures in the Shell or not</param>
			/// <param name="toExteriorEdges">Choose whether to use the exterior Edges of the Shell or not</param>
			/// <param name="toExteriorApertures">Choose whether to use any exterior Apertures of the Shell or not</param>
			/// <returns>The created dual graph</returns>
			static DualGraph_^ ByShell(
				Shell^ shell,
				[DefaultArgument("true")] bool direct,
				[DefaultArgument("false")] bool viaSharedEdges,
				[DefaultArgument("false")] bool viaSharedApertures,
				[DefaultArgument("false")] bool toExteriorEdges,
				[DefaultArgument("false")] bool toExteriorApertures);

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