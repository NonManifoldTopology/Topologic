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

	public ref class Cluster : Topology
	{

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		static Cluster^ ByTopology(List<Topology^>^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Cluster^ AddTopology(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Cluster^ RemoveTopology(Topology^ topology);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Shell^>^ Shells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Face^>^ Faces();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Vertex^>^ Vertices();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Cell^>^ Cells();

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