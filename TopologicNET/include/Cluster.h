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
		/// Creates a Cluster by a set of Topologies. 
		/// </summary>
		/// <param name="topologies">A set of Topologies</param>
		/// <returns name="Cluster">The created Cluster</returns>
		static Cluster^ ByTopologies(System::Collections::Generic::IEnumerable<Topology^>^ topologies);

		/// <summary>
		/// Adds Topology to a Cluster and returns the resulting Topology.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		Cluster^ AddTopology(Topology^ topology);

		/// <summary>
		/// Removes Topology from a Cluster and returns the resulting Topology.
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		Cluster^ RemoveTopology(Topology^ topology);

		/// <summary>
		/// Creates a geometry from Cluster.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the Shells constituent to the Cluster.
		/// </summary>
		/// <returns name="Shell[]">A list of Shells constituent to the Cluster</returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Returns the Faces constituent to the Cluster.
		/// </summary>
		/// <returns name="Face[]">A list of Faces constituent to the Cluster</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the Cluster.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires constituent to the Cluster</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the Cluster.
		/// </summary>
		/// <returns name="Edge[]">A list of Edges constituent to the Cluster</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the Cluster.
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices constituent to the Cluster</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Returns the Cells constituent to the Cluster.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells constituent to the Cluster</returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Returns the CellComplexes constituent to the Cluster.
		/// </summary>
		/// <returns name="CellComplex[]">A list of CellComplexes constituent to the Cluster</returns>
		property List<CellComplex^>^ CellComplexes
		{
			List<CellComplex^>^ get();
		}

		static int Type();

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

	protected:
		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Cluster>* m_pCoreCluster;
	};
}