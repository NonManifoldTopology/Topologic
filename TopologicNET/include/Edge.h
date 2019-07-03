#pragma once

#include "Topology.h"

#include <TopologicCore/include/Edge.h>

namespace Topologic {
	ref class Vertex;
	ref class Wire;
	ref class Geometry;

	/// <summary>
	/// An Edge is a one-dimensional entity defined by two vertices. It is important to note that while a topologic edge is made of two vertices, its geometry can be a curve with multiple control vertices.
	/// </summary>
	public ref class Edge : Topology
	{
	public:
		/// <summary>
		/// Creates a straight Edge by the startVertex and endVertex.
		/// </summary>
		/// <param name="startVertex">The start Vertex</param>
		/// <param name="endVertex">The end Vertex</param>
		/// <returns name="Edge">The created Edge</returns>
		static Edge^ ByStartVertexEndVertex(Vertex^ startVertex, Vertex^ endVertex);

#ifdef TOPOLOGIC_DYNAMO
		[IsVisibleInDynamoLibrary(false)]
#endif
		static Edge^ ByNurbsParameters(List<Vertex^>^ controlPoints, List<double>^ weights, List<double>^ knots, bool isRational, bool isPeriodic, int degree);

		/// <summary>
		/// Returns the Edges adjacent to the Edge.
		/// </summary>
		/// <returns name="Edge[]">A list of the Edges adjacent to the Edge</returns>
		property List<Edge^>^ AdjacentEdges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the start vertex of the edge.
		/// </summary>
		/// <returns name="Vertex"></returns>
		property Vertex^ StartVertex {
			Vertex^ get();
		}

		/// <summary>
		/// Get the end vertex of the edge.
		/// </summary>
		/// <returns name="Vertex"></returns>
		property Vertex^ EndVertex {
			Vertex^ get();
		}

		/// <summary>
		/// Returns the Vertices at the ends of the Edge.
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices at the ends of the Edge</returns>
		property List<Vertex^>^ Vertices {
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Returns the Wires incident to the Edge.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires incident to the Edge</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Return the shared Vertices between two edges.
		/// </summary>
		/// <param name="edge">Another Edge</param>
		/// <returns name="Vertex[]">The shared Vertices between the two Edges</returns>
		List<Vertex^>^ SharedVertices(Edge^ edge);

		/// <summary>
		/// Creates a geometry from Edge.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the type associated to Edge.
		/// </summary>
		/// <returns>The type associated to Edge</returns>
		static int Type();

	public protected:
		Edge();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreEdge"></param>
		Edge(const std::shared_ptr<TopologicCore::Edge>& kpCoreEdge);

#ifdef TOPOLOGIC_DYNAMO
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
		/// Create a curved edge by a Dynamo curve.
		/// </summary>
		/// <param name="curve">A Dynamo curve.</param>
		/// <returns name="Edge">The created Topologic edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve);

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
#else
		Topologic::Geometry^ Curve();
#endif

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Edge();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Edge>* m_pCoreEdge;
	};
}