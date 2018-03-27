#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Wire.h>

namespace TopoLogic {
	ref class Vertex;
	ref class Edge;
	ref class Face;

	public ref class Wire : Topology
	{
	public:
		/// <summary>
		/// Creates a wire by a list of edges.
		/// </summary>
		/// <param name="edges">The edges.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a TopoLogic edge.</exception>
		/// <returns>Te created TopoLogic wire</returns>
		static Wire^ ByEdges(List<Edge^>^ edges);

		/// <summary>
		/// Creates a wire by a polycurve (including a polygon)
		/// </summary>
		/// <param name="polycurve">The polycurve</param>
		/// <returns>The created TopoLogic wire</returns>
		static Wire^ ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve^ polycurve);

		/// <summary>
		/// Gets the list of edges constituent to this wire.
		/// </summary>
		/// <returns>The constituent edges</returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// Gets the list of faces that contain to this wire.
		/// </summary>
		/// <returns>The faces that contain this wire</returns>
		List<Face^>^ Faces();

		/// <summary>
		/// Gets the list of vertices constituent to this wire.
		/// </summary>
		/// <returns>The constituent vertices</returns>
		List<Vertex^>^ Vertices();

		/// <summary>
		/// Checks if the wire is closed.
		/// </summary>
		/// <returns>True if the wire is closed, otherwise false</returns>
		bool IsClosed();

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
		/// <param name="kpCoreWire"></param>
		Wire(const std::shared_ptr<TopoLogicCore::Wire>& kpCoreWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pEdges"></param>
		Wire(List<Edge^>^ pEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoPolycurve"></param>
		Wire(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopoLogicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Wire();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopoLogicCore::Wire>* m_pCoreWire;
	};
}