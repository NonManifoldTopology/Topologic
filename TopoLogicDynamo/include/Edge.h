#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Edge.h>

namespace TopoLogic {
	ref class Vertex;

	/// <summary>
	/// 
	/// </summary>
	public ref class Edge : Topology
	{
	public:
		/// <summary>
		/// Returns the start vertex of an edge. 
		/// </summary>
		/// <param name="topoLogicEdge">An edge.</param>
		/// <returns name="TopoLogic Vertex">
		/// "TopoLogic Vertex": the start vertex of an edge
		/// </returns>
		/// <returns name="Point">
		/// "Point": the Dynamo point counterpart of the start vertex
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertex", "Point" })]
		static Dictionary<String^, Object^>^ StartVertex(Edge^ topoLogicEdge);

		/// <summary>
		/// Returns the end vertex of an edge.
		/// </summary>
		/// <param name="topoLogicEdge">	An edge. </param>
		/// <returns name="TopoLogic Vertex">
		/// "TopoLogic Vertex": the end vertex of an edge
		/// </returns>
		/// <returns name="Point">
		/// "Point": the Dynamo point counterpart of the end vertex
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertex", "Point" })]
		static Dictionary<String^, Object^>^ EndVertex(Edge^ topoLogicEdge);


		/// <summary>
		/// Gets the list of wires incident to this edge.
		/// </summary>
		/// <param name="topoLogicEdge">	An edge. </param>
		/// <returns name="TopoLogic Wires">
		/// "TopoLogic Wires": the list of wires incident to this edge
		/// </returns>
		/// <returns name="PolyCurves">
		/// "PolyCurves": the Dynamo point counterpart of the end vertex
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wires", "PolyCurves" })]
		static Dictionary<String^, Object^>^ Wires(Edge^ topoLogicEdge);


		/// <summary>	A factory method that creates a curved edge by a Dynamo curve. </summary>
		/// <param name="curve">	A Dynamo curve. </param>
		/// <returns name="TopoLogic Edge">
		/// "TopoLogic Edge": the created TopoLogic edges
		/// </returns>
		/// <returns name="Curve">
		/// "Curve": the Dynamo curve counterpart of the edge
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edge", "Curve" })]
		static Dictionary<String^, Object^>^ ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve);


		/// <summary>
		/// A factory method that creates a straight edge by its start and end vertices.
		/// </summary>
		/// <exception cref="ArgumentException">	Thrown if any of the arguments is neither a Dynamo
		/// 										vertex nor a TopoLogic vertex. </exception>
		/// <exception cref="StdFail_NotDone">  	Thrown if OCCT fails to create an edge from this pair
		/// 										of vertices. Passed from the Edge::Edge(Vertex^, Vertex^) </exception>
		/// <param name="vertices">A list of vertices. The created edge will pass all vertices in this list.</param>
		/// <returns name="TopoLogic Edge">
		/// "TopoLogic Edge": the created TopoLogic edges
		/// </returns>
		/// <returns name="Line">
		/// "Line": the Dynamo line counterpart of the edge
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edge", "Line" })]
		static Dictionary<String^, Object^>^ ByVertices(List<Vertex^>^ vertices);

		/// <summary>
		/// Returns the shared vertex between two edges.
		/// </summary>
		/// <param name="topoLogicEdge1">	An edge. </param>
		/// <param name="topoLogicEdge2">	Another edge. </param>
		/// <returns name="TopoLogic Vertex">
		/// "TopoLogic Vertex": the shared vertex of an edge
		/// </returns>
		/// <returns name="Point">
		/// "Point": the Dynamo point counterpart of the end vertex
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertex", "Point" })]
		static Dictionary<String^, Object^>^ SharedVertex(Edge^ topoLogicEdge1, Edge^ topoLogicEdge2);

		property Object^ Geometry
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

	protected:
		/// <summary>
		/// 
		/// </summary>
		Edge();
		virtual ~Edge();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Edge* m_pCoreEdge;
	};
}