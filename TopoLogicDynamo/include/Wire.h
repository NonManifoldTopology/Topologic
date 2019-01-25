#pragma once

#include <Topology.h>

#include <TopologicCore/include/Wire.h>

namespace Topologic {
	ref class Vertex;
	ref class Edge;
	ref class Face;

	/// <summary>
	/// A Wire is a contiguous collection of Edges where adjacent Edges are connected by shared Vertices. It may be open or closed and may be manifold or non-manifold. 
	/// </summary>

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
		/// Get the constituent edges of the wire. NOTE: This method will automatically identify its loop order property.
		/// </summary>
		/// <returns name="Edge[]">The constituent edges</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the list of faces that contain the wire.
		/// </summary>
		/// <returns name="Face[]">The faces that contain the wire</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Gets the list constituent vertices of the wire. NOTE: This method will automatically identify its loop order property.
		/// </summary>
		/// <returns name="Vertex[]">The constituent vertices</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		int GetNumberOfBranches();

		/// <summary>
		/// Check if the wire is closed.
		/// </summary>
		/// <returns name="bool">True if the wire is closed, otherwise false</returns>
		property bool IsClosed
		{
			bool get();
		}

		property Object^ Geometry_
		{
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			virtual Object^ get() override;
		}

		static int Type();

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreWire"></param>
		Wire(const std::shared_ptr<TopologicCore::Wire>& kpCoreWire);

		/// <summary>
		/// Create a wire by a polycurve (including a polygon).
		/// </summary>
		/// <param name="polyCurve">The polycurve</param>
		/// <returns name="Wire">The created Topologic wire</returns>
		static Wire^ ByPolyCurve(Autodesk::DesignScript::Geometry::PolyCurve^ polyCurve);

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