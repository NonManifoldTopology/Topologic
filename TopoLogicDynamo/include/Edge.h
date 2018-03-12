#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Edge.h>

namespace TopoLogic {
	ref class Vertex;
	ref class Wire;

	/// <summary>
	/// 
	/// </summary>
	public ref class Edge : Topology
	{
	public:
		/// <summary>
		/// Creates a curved edge by a Dynamo curve.
		/// </summary>
		/// <param name="curve">A Dynamo curve.</param>
		/// <returns>The created TopoLogic edge.</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve);

		/// <summary>
		/// Creates an edge by a list of vertices. If the list only contains two vertices, a straight line will be created.
		/// If more than two are given, the edge will be interpolated through the vertices.
		/// Otherwise, an exception will be raised.
		/// </summary>
		/// <param name="vertices">A list of vertices. The created edge will pass all vertices in this list.</param>
		/// <exception cref="ArgumentException">	Thrown if any of the arguments is neither a Dynamo
		/// 										vertex nor a TopoLogic vertex. </exception>
		/// <exception cref="StdFail_NotDone">  	Thrown if OCCT fails to create an edge from this pair
		/// 										of vertices. Passed from the Edge::Edge(Vertex^, Vertex^) </exception>
		/// <returns>The created TopoLogic edges</returns>
		static Edge^ ByVertices(List<Vertex^>^ vertices);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Vertex^ StartVertex();

		/// <summary>
		/// Returns the end vertex of an edge.
		/// </summary>
		/// <returns>The end vertex of the edge</returns>
		Vertex^ EndVertex();

		/// <summary>
		/// Gets the list of wires incident to this edge.
		/// </summary>
		/// <returns>The list of wires incident to this edge</returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// Returns the shared vertex between two edges.
		/// </summary>
		/// <param name="edge">An edge.</param>
		/// <returns>the shared vertex of an edge</returns>
		Vertex^ SharedVertex(Edge^ edge);

		property Object^ Geometry
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
		/// <param name="kpCoreEdge"></param>
		Edge(TopoLogicCore::Edge* const kpCoreEdge);

		/// <summary>
		/// 
		/// </summary>
		Edge(Autodesk::DesignScript::Geometry::Curve^ curve);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Curve^ Curve();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCurve"></param>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Curve^ Curve(Handle(Geom_Curve) pOcctCurve, const double u0, const double u1);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::TopologicalQuery* GetCoreTopologicalQuery() override;

	protected:
		virtual ~Edge();

		/// <summary>
		/// Initialises the edge given a NurbsCurve argument. Called by the respective constructor.
		/// </summary>
		/// <exception cref="Standard_ConstructionError">	Thrown if OCCT fails to initialise the
		/// 												underlying curve. </exception>
		/// <exception cref="StdFail_NotDone">			 	Thrown if OCCT fails to create an edge from
		/// 												the curve. </exception>
		///
		/// <param name="pDynamoNurbsCurve">	A Dynamo NURBS curve. </param>
		void Init(Autodesk::DesignScript::Geometry::NurbsCurve^ pDynamoNurbsCurve);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoLine"></param>
		void Init(Autodesk::DesignScript::Geometry::Line^ pDynamoLine);

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Edge* m_pCoreEdge;
	};
}