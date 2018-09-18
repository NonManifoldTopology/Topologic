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
		/// Create a wire by a list of edges.
		/// </summary>
		/// <param name="edges">The edges.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a Topologic edge.</exception>
		/// <returns name="Wire">The created Topologic wire</returns>
		static Wire^ ByEdges(System::Collections::Generic::IEnumerable<Edge^>^ edges);

		/// <summary>
		/// Create a wire by a polycurve (including a polygon). NOTE: This node will be replaced by a single Topology.ByGeometry() node.
		/// </summary>
		/// <param name="polyCurve">The polycurve</param>
		/// <returns name="Wire">The created Topologic wire</returns>
		static Wire^ ByPolyCurve_(Autodesk::DesignScript::Geometry::PolyCurve^ polyCurve);

		/// <summary>
		/// Get the constituent edges of the wire. NOTE: This method will automatically identify its loop order property.
		/// </summary>
		/// <param name="ordered">Does the wire has loop order?</param>
		/// <returns name="Edge[]">The constituent edges</returns>
		List<Edge^>^ Edges_(bool ordered);

		/// <summary>
		/// Get the list of faces that contain the wire. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Face[]">The faces that contain the wire</returns>
		List<Face^>^ Faces_(Topology^ hostTopology);

		/// <summary>
		/// Gets the list constituent vertices of the wire. NOTE: This method will automatically identify its loop order property.
		/// </summary>
		/// <param name="ordered">Does the wire has loop order?</param>
		/// <returns name="Vertex[]">The constituent vertices</returns>
		List<Vertex^>^ Vertices_(bool ordered);

		/// <summary>
		/// Check if the wire is closed.
		/// </summary>
		/// <returns name="bool">True if the wire is closed, otherwise false</returns>
		property bool IsClosed
		{
			bool get();
		}

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