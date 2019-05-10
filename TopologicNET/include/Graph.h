#pragma once

#include <TopologicCore/include/Graph.h>

#ifdef TOPOLOGIC_DYNAMO
using namespace Autodesk::DesignScript::Runtime;
#endif

using namespace System;
using namespace System::Collections::Generic;

namespace Topologic {
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Topology;

	/// <summary>
	/// A Graph consists of a set of Vertices and a set of Edges that connect the Vertices. It can be a Wire, Vertex, Edge or Cluster and is defined by the input Topology. It can be manifold or non-manifold.
	/// </summary>
	public ref class Graph
	{
	public:

		/// <summary>
		/// Creates a Graph from any topology.
		/// </summary>
		/// <param name="direct"></param>
		/// <param name="viaSharedTopologies">Use shared Topologies</param>
		/// <param name="viaSharedApertures">Use shared Apertures</param>
		/// <param name="toExteriorTopologies">Use exterior Topologies</param>
		/// <param name="toExteriorApertures">Use exterior Apertures</param>
		/// <returns name="Graph">The created Graph</returns>
#ifdef TOPOLOGIC_DYNAMO
		static Graph^ ByTopology(
			Topology^ topology,
			[DefaultArgument("true")] bool direct,
			[DefaultArgument("false")] bool viaSharedTopologies,
			[DefaultArgument("false")] bool viaSharedApertures,
			[DefaultArgument("false")] bool toExteriorTopologies,
			[DefaultArgument("false")] bool toExteriorApertures);
#else
		static Graph^ ByTopology(
			Topology^ topology,
			bool direct,
			bool viaSharedTopologies,
			bool viaSharedApertures,
			bool toExteriorTopologies,
			bool toExteriorApertures);
#endif

		/// <summary>
		/// Returns a list of all the Vertices in a Graph.
		/// </summary>
		/// <returns name="Vertex[]">The Vertices in this Graph</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Returns a list of Vertices that are not connected by Edges.
		/// </summary>
		/// <returns name="Vertex[]">The isolated Vertices in this Graph</returns>
		property List<Vertex^>^ IsolatedVertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Returns all the Edges of a Graph.
		/// </summary>
		/// <returns name="Edge[]">The Edges in this Graph</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Topology of the Graph.
		/// </summary>
		/// <returns name="Topology">The Topology of the Graph</returns>
		property Topology^ Topology
		{
			Topologic::Topology^ get();
		}

		/// <summary>
		/// Returns a list of Vertices in the Graph located at the input coordinates within the input tolerance distance.
		/// </summary>
		/// <param name="x">The X coordinate</param>
		/// <param name="y">The Y coordinate</param>
		/// <param name="z">The Z coordinate</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Vertex[]">The Vertices at the given coordinates</returns>
		List<Vertex^>^ VerticesAtCoordinates(
			double x, double y, double z, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Adds the list of Vertices to the Graph. If a tolerance is specified, the Vertices will be merged with existing Vertices within the input tolerance distance. A zero or null tolerance means that the Vertices will be added to the Graph regardless of their overlap with existing Vertices.
		/// </summary>
		/// <param name="vertices">The Vertices to be added</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Graph">The Graph after the Vertices are added</returns>
		Graph^ AddVertices(
			List<Vertex^>^ vertices,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Adds the list of Edges to the Graph. The Vertices of the Edges are merged with existing Vertices if they fall within the input tolerance. No new Edges will be added if they already exist. A zero or null tolerance means that the Vertices of the input Edges will be added to the Graph regardless of their overlap with existing Vertices.
		/// </summary>
		/// <param name="edges">The Edges to be added</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Graph">The Graph after the Edges are added</returns>
		Graph^ AddEdges(
			List<Edge^>^ edges,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Connects the two input Vertices with an Edge.
		/// </summary>
		/// <param name="vertex1">The first Vertex</param>
		/// <param name="vertex2">The second Vertex</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Graph">The Graph after the Vertices are connected</returns>
		Graph^ Connect(
			Vertex^ vertex1, 
			Vertex^ vertex2, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Returns the number of Edges connected to the input Vertex.
		/// </summary>
		/// <param name="vertex">The input Vertex</param>
		/// <returns name="int">The degree of a Vertex</returns>
		int Degree(Vertex^ vertex);

		/// <summary>
		/// Returns a list of Vertices that are connected to the input Vertex by an Edge.
		/// </summary>
		/// <param name="vertex">The input Vertex</param>
		/// <returns name="Vertex[]">The Vertices adjacent to a Vertex</returns>
		List<Vertex^>^ AdjacentVertices(Vertex^ vertex);

		/// <summary>
		/// Returns True if the input Graph contains the input Vertex. Returns False otherwise.
		/// </summary>
		/// <param name="vertex">The input Vertex</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="bool">True if the input Graph contains the input Vertex, False otherwise</returns>
		bool ContainsVertex(Vertex^ vertex, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Returns True if the input Graph contains the input Edge. Returns False otherwise.
		/// </summary>
		/// <param name="edge">The input Edge</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="bool">True if the input Graph contains the input Edge, False otherwise</returns>
		bool ContainsEdge(Edge^ edge, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Returns a list of Vertex degrees in a non-increasing order.
		/// </summary>
		/// <returns name="int[]">A list of Vertex degrees in a non-increasing order</returns>
		property List<int>^ DegreeSequence
		{
			List<int>^ get();
		}

		/// <summary>
		/// Returns the ratio of the number of Edges to the total number of Edges the Graph could have.
		/// </summary>
		/// <returns name="double">The graph's density</returns>
		property double Density
		{
			double get();
		}

		/// <summary>
		/// Returns True if the Graph has a density of 1. Returns False otherwise.
		/// </summary>
		/// <returns name="bool">True if the Graph has a density of 1, False otherwise</returns>
		property bool IsComplete
		{
			bool get();
		}

		/// <summary>
		/// Returns the minimum Vertex degree in a Graph.
		/// </summary>
		/// <returns name="int">The minimum Vertex degree in a Graph</returns>
		property int MinimumDelta
		{
			int get();
		}

		/// <summary>
		/// Returns the maximum Vertex degree in a Graph.
		/// </summary>
		/// <returns name="int">The maximum Vertex degree in a Graph</returns>
		property int MaximumDelta
		{
			int get();
		}

		/// <summary>
		/// Returns the number of Edges of the shortest path between the most distanced Vertices.
		/// </summary>
		/// <returns name="int">The diameter of the Graph</returns>
		property int Diameter
		{
			int get();
		}

		/// <summary>
		/// Returns all paths it could find, within the input number of seconds, that connect the two input Vertices.
		/// </summary>
		/// <param name="startVertex">The start Vertex</param>
		/// <param name="endVertex">The end Vertex</param>
		/// <param name="timeLimitInSeconds">Time limit in seconds</param>
		/// <returns name="Wire[]">The paths</returns>
		List<Wire^>^ AllPaths(
			Vertex^ startVertex, 
			Vertex^ endVertex, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")]
#endif
			Nullable<int> timeLimitInSeconds);

		/// <summary>
		/// Returns the first Path found between the input vertices.
		/// </summary>
		/// <param name="startVertex">The start Vertex</param>
		/// <param name="endVertex">The end Vertex</param>
		/// <returns name="Wire">The path</returns>
		Wire^ Path(Vertex^ startVertex, Vertex^ endVertex);


		/// <summary>
		/// Returns the path with the fewest number of Edges between the two input Vertices.
		/// </summary>
		/// <param name="startVertex">The start Vertex</param>
		/// <param name="endVertex">The end Vertex</param>
		/// <param name="vertexKey">The key for the Vertex cost</param>
		/// <param name="edgeKey">The key for the Edge cost</param>
		/// <returns name="Wire">The shortest path</returns>
		Wire^ ShortestPath(Vertex^ startVertex, Vertex^ endVertex,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")]
#endif
		String^ vertexKey,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("null")]
#endif
		String^ edgeKey);

		/// <summary>
		/// Returns the number of Edges of the shortest path connecting the two input Vertices.
		/// </summary>
		/// <param name="startVertex">The start Vertex</param>
		/// <param name="endVertex">The end Vertex</param>
		/// <returns name="double">The number of Edges connecting the two input Vertices</returns>
		double Distance(Vertex^ startVertex, Vertex^ endVertex);

		/// <summary>
		/// Returns True if the input sequence satisfies the Erdoes Gallai theorem. It returns False otherwise.
		/// </summary>
		/// <param name="sequence">The input sequence</param>
		/// <returns name="bool">True if the input sequence satisfies the Erdoes Gallai theorem, False otherwise</returns>
		bool IsErdoesGallai(List<int>^ sequence);

		/// <summary>
		/// Removes the input Vertices from the Graph.
		/// </summary>
		/// <param name="vertices">The Vertices to be removed</param>
		/// <returns name="Graph">The Graph after the Vertices are removed</returns>
		Graph^ RemoveVertices(List<Vertex^>^ vertices);

		/// <summary>
		/// Removes the input Edges from the Graph.
		/// </summary>
		/// <param name="edges">The Edges to be removed</param>
		/// <returns name="Graph">The Graph after the Edges are removed</returns>
		Graph^ RemoveEdges(List<Edge^>^ edges);

		/// <summary>
		/// Returns the Edge, if one exists, that connects the two input Vertices.
		/// </summary>
		/// <param name="vertex1">The first Vertex</param>
		/// <param name="vertex2">The second Vertex</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Edge">The Edge connecting the two Vertices</returns>
		Edge^ EdgeAtVertices(Vertex^ vertex1, Vertex^ vertex2,
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

		/// <summary>
		/// Returns a list of all Edges connected to the input Vertex.
		/// </summary>
		/// <param name="vertex">A Vertex</param>
		/// <param name="tolerance">A positive tolerance value</param>
		/// <returns name="Edge[]">The Edges connected to the input Vertex</returns>
		List<Edge^>^ IncidentEdges(Vertex^ vertex, 
#ifdef TOPOLOGIC_DYNAMO
			[Autodesk::DesignScript::Runtime::DefaultArgument("0.0001")]
#endif
			double tolerance);

	public protected:
		Graph(const std::shared_ptr<TopologicCore::Graph>& kpCoreGraph);

	protected:
		virtual ~Graph();

		std::shared_ptr<TopologicCore::Graph>* m_pCoreGraph;
	};
}