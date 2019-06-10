#pragma once

#include "Topology.h"

#include <TopologicCore/include/Edge.h>

namespace Topologic {
	ref class Vertex;
	ref class Wire;

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

		/// <summary>
		/// Returns the Edges adjacent to the Edge.
		/// </summary>
		/// <returns name="Edge[]">A list of the Edges adjacent to the Edge</returns>
		property List<Edge^>^ AdjacentEdges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the start Vertex of the Edge.
		/// </summary>
		/// <returns name="Vertex">The start Vertex of the Edge</returns>
		property Vertex^ StartVertex {
			Vertex^ get();
		}

		/// <summary>
		/// Returns the end Vertex of the Edge.
		/// </summary>
		/// <returns name="Vertex">The end Vertex of the Edge</returns>
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
		/// Returns the shared Vertices between two Edges.
		/// </summary>
		/// <param name="edge">Another Edge</param>
		/// <returns name="Vertex[]">A list of shared Vertices between the two Edges</returns>
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
		/// Creates a curved Edge by a Dynamo curve.
		/// </summary>
		/// <param name="curve">A Dynamo curve</param>
		/// <returns name="Edge">The created Topologic Edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Curve^ curve);

		/// <summary>
		/// Creates an Edge by a NurbsCurve. 
		/// </summary>
		/// <param name="pDynamoNurbsCurve">A Dynamo NURBS curve</param>
		/// <returns>The created Topologic Edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::NurbsCurve^ pDynamoNurbsCurve);

		/// <summary>
		/// Creates an Edge by a Dynamo circle.
		/// </summary>
		/// <param name="pDynamoCircle">A Dynamo circle</param>
		/// <returns>The created Topologic Edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Circle^ pDynamoCircle);

		/// <summary>
		/// Creates an Edge by a Dynamo line.
		/// </summary>
		/// <param name="pDynamoLine">A Dynamo line</param>
		/// <returns>The created Topologic Edge</returns>
		static Edge^ ByCurve(Autodesk::DesignScript::Geometry::Line^ pDynamoLine);
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