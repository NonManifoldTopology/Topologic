#pragma once

#include <Topology.h>

#include <TopologicCore/include/Edge.h>

namespace Topologic {
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
		/// <returns name="Edge">The created Topologic edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve);

		/// <summary>
		/// Creates an edge by a list of vertices. If the list only contains two vertices, a straight line will be created.
		/// If more than two are given, the edge will be interpolated through the vertices.
		/// Otherwise, an exception will be raised.
		/// </summary>
		/// <param name="vertices">A list of vertices. The created edge will pass all vertices in this list.</param>
		/// <returns name="Edge">The created Topologic edge</returns>
		static Edge^ ByVertices(List<Vertex^>^ vertices);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		List<Vertex^>^ Vertices();

		/// <summary>
		/// Gets the list of wires incident to this edge.
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="Wire[]">The list of wires incident to this edge</returns>
		List<Wire^>^ Wires(Topology^ parentTopology);

		/// <summary>
		/// Returns the shared vertex between two edges.
		/// </summary>
		/// <param name="edge">An edge.</param>
		/// <returns name="Vertex">the shared vertex of an edge</returns>
		Vertex^ SharedVertex(Edge^ edge);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns name="double"></returns>
		double ParameterAtPoint(Vertex^ vertex);

		/// <summary>
		/// Returns the point at the input parameter
		/// </summary>
		/// <param name="parameter">The input parameter</param>
		/// <returns name="Vertex">The sample point</returns>
		Vertex^ PointAtParameter(double parameter);

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
		Edge(const std::shared_ptr<TopologicCore::Edge>& kpCoreEdge);

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
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Edge();

		/// <summary>
		/// Initialises the edge given a NurbsCurve argument. Called by the respective constructor.
		/// </summary>
		/// <param name="pDynamoNurbsCurve">	A Dynamo NURBS curve. </param>
		void Init(Autodesk::DesignScript::Geometry::NurbsCurve^ pDynamoNurbsCurve);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoCircle"></param>
		void Init(Autodesk::DesignScript::Geometry::Circle^ pDynamoCircle);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoLine"></param>
		void Init(Autodesk::DesignScript::Geometry::Line^ pDynamoLine);

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Edge>* m_pCoreEdge;
	};
}