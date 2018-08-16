#pragma once

#include <Topology.h>

#include <TopologicCore/include/Wire.h>

namespace Topologic {
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
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a Topologic edge.</exception>
		/// <returns name="Wire">Te created Topologic wire</returns>
		static Wire^ ByEdges(System::Collections::Generic::IEnumerable<Edge^>^ edges);

		/// <summary>
		/// Creates a wire by a polycurve (including a polygon)
		/// </summary>
		/// <param name="polycurve">The polycurve</param>
		/// <returns name="Wire">The created Topologic wire</returns>
		static Wire^ ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve^ polycurve);

		/// <summary>
		/// Gets the list of edges constituent to this wire.
		/// </summary>
		/// <param name="hasOrder">Does the wire has loop order?</param>
		/// <returns name="Edge[]">The constituent edges</returns>
		List<Edge^>^ Edges(bool hasOrder);

		/// <summary>
		/// Gets the list of faces that contain to this wire.
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="Face[]">The faces that contain this wire</returns>
		List<Face^>^ Faces(Topology^ parentTopology);

		/// <summary>
		/// Gets the list of vertices constituent to this wire.
		/// </summary>
		/// <param name="hasOrder">Does the wire has loop order?</param>
		/// <returns name="Vertex[]">The constituent vertices</returns>
		List<Vertex^>^ Vertices(bool hasOrder);

		/// <summary>
		/// Checks if the wire is closed.
		/// </summary>
		/// <returns name="bool">True if the wire is closed, otherwise false</returns>
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
		Wire(const std::shared_ptr<TopologicCore::Wire>& kpCoreWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pEdges"></param>
		Wire(System::Collections::Generic::IEnumerable<Edge^>^ pEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoPolycurve"></param>
		Wire(Autodesk::DesignScript::Geometry::PolyCurve^ pDynamoPolycurve);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Wire();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Wire>* m_pCoreWire;
	};
}