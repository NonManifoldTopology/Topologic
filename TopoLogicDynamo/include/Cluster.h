#pragma once

#include <Topology.h>

#include <TopologicCore/include/Cluster.h>

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Shell;
	ref class Cell;
	ref class CellComplex;

	/// <summary>
	/// A Cluster is a collection of any topologic entities. It may be contiguous or not and may be manifold or non-manifold. Clusters can be nested within other Clusters.
	/// </summary>

	public ref class Cluster : Topology
	{

	public:
		/// <summary>
		/// Create a cluster by a set of topologies. 
		/// </summary>
		/// <param name="topologies"></param>
		/// <returns name="Cluster"></returns>
		static Cluster^ ByTopologies(System::Collections::Generic::IEnumerable<Topology^>^ topologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		Cluster^ AddTopology(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		Cluster^ RemoveTopology(Topology^ topology);

		property Object^ Geometry_
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

		/// <summary>
		/// Get the shells constituent to the cluster.
		/// </summary>
		/// <returns name="Shell[]"></returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Get the faces constituent to the cluster.
		/// </summary>
		/// <returns name="Face[]"></returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the wires constituent to the cluster.
		/// </summary>
		/// <returns name="Wire[]"></returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Get the edges constituent to the cluster.
		/// </summary>
		/// <returns name="Edge[]"></returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the vertices constituent to the cluster.
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Get the cells constituent to the cluster.
		/// </summary>
		/// <returns name="Cell[]"></returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Get the cellComplexes constituent to the cluster.
		/// </summary>
		/// <returns name="CellComplex[]"></returns>
		property List<CellComplex^>^ CellComplexes
		{
			List<CellComplex^>^ get();
		}

		//static int ClusterType() { return 0; }

	public protected:
		Cluster();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreCluster"></param>
		Cluster(const std::shared_ptr<TopologicCore::Cluster>& kpCoreCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

		static int Type();

	protected:
		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Cluster>* m_pCoreCluster;
	};
}