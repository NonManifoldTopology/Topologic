#pragma once

#include <Wire.h>

#include <TopologicExtension/include/Graph.h>

namespace Topologic
{
	ref class CellComplex;

	/// <summary>
	/// Extension currently includes the Graph class.
	/// </summary>

	namespace Extension
	{
		/// <summary>
		/// A Graph consists of a set of vertices and a set of edges that connect the vertices. It can be a Wire, Vertex, Edge or Cluster and is defined by the input Topology. It can be manifold or non-manifold.
		/// </summary>
		public ref class Graph : Wire
		{
		public:
			/// <summary>
			/// Creates a dual graph by a CellComplex. Options to be selected for the creation of the dual graph is the use of cells, non-manifold faces, manifold faces and apertures.
			/// </summary>
			/// <param name="cellComplex">The cell complex to be used for the creation of the dual graph</param>
			/// <param name="useCells">Choose whether to use the cells of the cell complex or not</param>
			/// <param name="useNonManifoldFaces">Choose whether to use the non-manifold faces of the cell complex or not</param>
			/// <param name="useManifoldFaces">Choose whether to use the manifold faces of the cell complex or not</param>
			/// <param name="useApertures">Choose whether to use the apertures of the cell complex or not</param>
			/// <returns>The created dual graph</returns>
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