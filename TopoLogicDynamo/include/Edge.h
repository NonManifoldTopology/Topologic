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
		/// Create a straight edge by the startVertex and endVertex.
		/// </summary>
		/// <param name="startVertex"></param>
		/// <param name="endVertex"></param>
		/// <returns name="Edge"></returns>
		static Edge^ ByStartVertexEndVertex(Vertex^ startVertex, Vertex^ endVertex);

		/// <summary>
		/// Get the edges adjacent to the edge. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Edge[]">The edges adjacent to the edge</returns>
		List<Edge^>^ AdjacentEdges_(Topology^ hostTopology);

		/// <summary>
		/// Get the vertices at the ends of the edge.
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		property List<Vertex^>^ Vertices {
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Get the wires incident to the edge. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Wire[]">The list of wires incident to the edge</returns>
		List<Wire^>^ Wires_(Topology^ hostTopology);

		/// <summary>
		/// Return the shared vertex between two edges.
		/// </summary>
		/// <param name="edge">An edge.</param>
		/// <returns name="Vertex">the shared vertex of an edge</returns>
		Vertex^ SharedVertex(Edge^ edge);

		/*/// <summary>
		/// Create an edge by a list of vertices. If the list only contains two vertices, a straight line will be created.
		/// If more than two are given, the edge will be interpolated through the vertices.
		/// Otherwise, an exception will be raised.
		/// </summary>
		/// <param name="vertices">A list of vertices. The created edge will pass all vertices in this list.</param>
		/// <returns name="Edge">The created Topologic edge</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Edge^ ByVertices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="centerPoint"></param>
		/// <param name="radius"></param>
		/// <param name="xAxisX"></param>
		/// <param name="xAxisY"></param>
		/// <param name="xAxisZ"></param>
		/// <param name="normalX"></param>
		/// <param name="normalY"></param>
		/// <param name="normalZ"></param>
		/// <returns></returns>
		/*[IsVisibleInDynamoLibrary(false)]
		static Edge^ ByCircle(
			Vertex^ centerPoint, double radius,
			double xAxisX, double xAxisY, double xAxisZ,
			double normalX, double normalY, double normalZ);*/

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns name="double"></returns>
		/*[IsVisibleInDynamoLibrary(false)]
		double ParameterAtVertex(Vertex^ vertex);*/

		/// <summary>
		/// Return the point at the input parameter
		/// </summary>
		/// <param name="parameter">The input parameter</param>
		/// <returns name="Vertex">The sample point</returns>
		/*[IsVisibleInDynamoLibrary(false)]
		Vertex^ VertexAtParameter(double parameter);*/

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

		/// <summary>
		/// Create a curved edge by a Dynamo curve. NOTE: This node will be replaced by a single Topology.ByGeometry() node.
		/// </summary>
		/// <param name="curve">A Dynamo curve.</param>
		/// <returns name="Edge">The created Topologic edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve);

	protected:
		virtual ~Edge();

		/// <summary>
		/// Initialises the edge given a NurbsCurve argument. 
		/// </summary>
		/// <param name="pDynamoNurbsCurve"></param>
		/// <returns></returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::NurbsCurve^ pDynamoNurbsCurve);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoCircle"></param>
		/// <returns></returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Circle^ pDynamoCircle);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoLine"></param>
		/// <returns></returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Line^ pDynamoLine);

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Edge>* m_pCoreEdge;
	};
}