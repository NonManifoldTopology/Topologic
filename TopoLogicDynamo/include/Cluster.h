#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Cluster.h>

namespace TopoLogic
{
	ref class Vertex;
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
		/// <param name="cluster"></param>
		/// <param name="topology"></param>
		/// <returns></returns>
		static Cluster^ Add(Cluster^ cluster, Topology^ topology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cluster"></param>
		/// <param name="topology"></param>
		/// <returns></returns>
		static Cluster^ Remove(Cluster^ cluster, Topology^ topology);

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
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Shells", "Polysurfaces" })]
		static Dictionary<String^, Object^>^ Shells(Cluster^ topoLogicCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ Faces(Cluster^ topoLogicCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wires", "PolyCurves" })]
		static Dictionary<String^, Object^>^ Wires(Cluster^ topoLogicCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ Edges(Cluster^ topoLogicCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ Vertices(Cluster^ topoLogicCluster);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cells", "Solids" })]
		static Dictionary<String^, Object^>^ Cells(Cluster^ topoLogicCluster);


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
		virtual TopoLogicCore::Topology* GetCoreTopology() override;


	protected:
		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Cluster* m_pCoreCluster;
	};
}