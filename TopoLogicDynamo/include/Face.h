#pragma once

#include <Topology.h>

#include <TopologicCore/include/Face.h>

namespace Topologic {
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Shell;
	ref class Cell;

	/// <summary>
	/// 
	/// </summary>
	public ref class Face : Topology
	{
	public:
		/// <summary>
		/// Get the faces adjacent to the face. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Face[]">The faces adjacent to the face</returns>
		List<Face^>^ AdjacentFaces_(Topology^ hostTopology);

		/// <summary>
		/// Get the cells incident to the face. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Cell[]">The cells incident to the face</returns>
		List<Cell^>^ Cells_(Topology^ hostTopology);

		/// <summary>
		/// Gets the shells incident to the face. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Shell[]">The shells incident to the face</returns>
		List<Shell^>^ Shells_(Topology^ hostTopology);

		/// <summary>
		/// Gets the constituent vertices of the face. 
		/// </summary>
		/// <returns name="Vertex[]">The vertices consituent to the face</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Get the edges constituent to the face.
		/// </summary>
		/// <returns name="Edge[]">The constituent edges</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the wires constituent to the face.
		/// </summary>
		/// <returns name="Wire[]">The constituent wires</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="double"></returns>
		[IsVisibleInDynamoLibrary(false)]
		double Area();

		/// <summary>
		/// Create a face by a closed wire. NOTE: This node currently can only create a planar face. To create a curved face, please use Topology.ByGeometry().
		/// </summary>
		/// <param name="wire">A closed wire. Must be (and internally verified if it is) a Dynamo polygon or a Topologic wire.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is neither a Dynamo polygon nor a Topologic wire</exception>
		/// <returns name="Face">/// </returns>
		static Face^ ByWire(Wire^ wire);

		/// <summary>
		/// Create a face by an external boundary and internal boundaries.
		/// </summary>
		/// <param name="externalBoundary"></param>
		/// <param name="internalBoundaries"></param>
		/// <returns name="Face"></returns>
		static Face^ ByExternalInternalBoundaries(Wire^ externalBoundary, System::Collections::Generic::IEnumerable<Wire^>^ internalBoundaries);

		/// <summary>
		/// Create a face by a list of edges.
		/// </summary>
		/// <param name="edges">The edges. </param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a Topologic edge</exception>
		/// <returns name="Face">The created face</returns>
		static Face^ ByEdges(System::Collections::Generic::IEnumerable<Edge^>^ edges);

		/// <summary>
		/// Create a face by a surface. NOTE: This node will be replaced by a single Topology.ByGeometry() node.
		/// </summary>
		/// <param name="surface">The surface</param>
		/// <returns name="Face">The created face</returns>
		static Face^ BySurface_(Autodesk::DesignScript::Geometry::Surface^ surface);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertices"></param>
		/// <returns name="Face"></returns>
		//[IsVisibleInDynamoLibrary(false)]
		static Face^ ByVertices(System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<Vertex^>^>^ vertices);

		/// <summary>
		/// Return the shared edges between two faces. 
		/// </summary>
		/// <param name="face">Another face</param>
		/// <returns name="Edge[]">The shared edges</returns>
		List<Edge^>^ SharedEdges(Face^ face);

		/// <summary>
		/// Return the shared vertices between two faces.
		/// </summary>
		/// <param name="face">Another face</param>
		/// <returns name="Vertex[]">The shared vertices</returns>
		List<Vertex^>^ SharedVertices(Face^ face);

		/// <summary>
		/// Return the external boundary (wire) of the face.
		/// </summary>
		/// <returns name="Wire"></returns>
		property Wire^ ExternalBoundary
		{
			Wire^ get();
		}

		/// <summary>
		/// Return the internal boundaries (wires) of the face.
		/// </summary>
		/// <returns name="Wire[]"></returns>
		property List<Wire^>^ InternalBoundaries
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Add internal boundaries to a face.
		/// </summary>
		/// <param name="internalBoundaries"></param>
		/// <returns name="Face"></returns>
		Face^ AddInternalBoundaries(List<Wire^>^ internalBoundaries);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns name="UV"></returns>
		[IsVisibleInDynamoLibrary(false)]
		Autodesk::DesignScript::Geometry::UV^ UVParameterAtVertex(Vertex^ vertex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="uv"></param>
		/// <returns name="Vertex"></returns>
		[IsVisibleInDynamoLibrary(false)]
		Vertex^ VertexAtParameter(Autodesk::DesignScript::Geometry::UV^ uv);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="uv"></param>
		/// <returns></returns>
		[IsVisibleInDynamoLibrary(false)]
		Autodesk::DesignScript::Geometry::Vector^ NormalAtParameter(Autodesk::DesignScript::Geometry::UV^ uv);

		/// <summary>
		/// Trim a face with a wire. The portion of the face inside the wire will be returned.
		/// </summary>
		/// <param name="wire"></param>
		/// <returns name="Face"></returns>
		[IsVisibleInDynamoLibrary(false)]
		Face^ TrimByWire_(Wire^ wire);

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
		/// <param name="kpCoreFace"></param>
		Face(const std::shared_ptr<TopologicCore::Face>& kpCoreFace);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Surface^ Surface();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Autodesk::DesignScript::Geometry::Mesh^ TriangulatedMesh();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Face();

		/// <summary>
		/// Initialises the face given a NurbsSurface argument. Called by the respective constructor.
		/// </summary>
		/// <param name="pDynamoNurbsSurface">A Dynamo NURBS surface</param>
		/// <returns></returns>
		static Face^ BySurface(Autodesk::DesignScript::Geometry::NurbsSurface^ pDynamoNurbsSurface,
			array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoPerimeterCurves);

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Face>* m_pCoreFace;
	};
}