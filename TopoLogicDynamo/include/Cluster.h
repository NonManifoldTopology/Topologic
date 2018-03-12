#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Cluster.h>

namespace TopoLogic
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
		Cluster^ Add(Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topology"></param>
		/// <returns></returns>
		Cluster^ Remove(Topology^ topology);

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
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreCell"></param>
		Cluster(TopoLogicCore::Cluster* const kpCoreCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::TopologicalQuery* GetCoreTopologicalQuery() override;


	protected:
		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Cluster* m_pCoreCluster;
	};
}