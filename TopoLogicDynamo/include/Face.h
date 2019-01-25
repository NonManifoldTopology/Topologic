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
	/// A Face is a two-dimensional region defined by a collection of closed Wires. The geometry of a face can be flat or undulating.
	/// </summary>
	public ref class Face : Topology
	{
	public:
		/// <summary>
		/// Returns the Faces adjacent to the Face.
		/// </summary>
		/// <returns name="Face[]">A list of Faces adjacent to the Face</returns>
		property List<Face^>^ AdjacentFaces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Returns the Cells incident to the Face.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells incident to the Face</returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Returns the Shells incident to the Face.
		/// </summary>
		/// <returns name="Shell[]">A list of Shells incident to the Face</returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the Face. 
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices constituent to the Face</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the Face.
		/// </summary>
		/// <returns name="Edge[]">A list of Edges constituent to the Face</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the Face.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires constituent to the Face</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Creates a Face by a closed planar Wire.
		/// </summary>
		/// <param name="wire">A closed planar Wire</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is neither a Dynamo polygon nor a Topologic Wire</exception>
		/// <returns name="Face">The created Face</returns>
		static Face^ ByWire(Wire^ wire);

		/// <summary>
		/// Creates a Face by an external boundary (Wire) and internal boundaries (Wires).
		/// </summary>
		/// <param name="externalBoundary">An external Wire</param>
		/// <param name="internalBoundaries">A set of internal Wires</param>
		/// <returns name="Face">The created Face</returns>
		static Face^ ByExternalInternalBoundaries(Wire^ externalBoundary, System::Collections::Generic::IEnumerable<Wire^>^ internalBoundaries);

		/// <summary>
		/// Creates a Face by a list of Edges.
		/// </summary>
		/// <param name="edges">A list of Edges</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a Topologic Edge</exception>
		/// <returns name="Face">The created Face</returns>
		static Face^ ByEdges(System::Collections::Generic::IEnumerable<Edge^>^ edges);

		/// <summary>
		/// Returns the shared Edges between two Faces. 
		/// </summary>
		/// <param name="face">Another Face</param>
		/// <returns name="Edge[]">A list of shared Edges</returns>
		List<Edge^>^ SharedEdges(Face^ face);

		/// <summary>
		/// Returns the shared Vertices between two Faces.
		/// </summary>
		/// <param name="face">Another Face</param>
		/// <returns name="Vertex[]">A list of shared Vertices</returns>
		List<Vertex^>^ SharedVertices(Face^ face);

		/// <summary>
		/// Returns the external boundary (Wire) of the Face.
		/// </summary>
		/// <returns name="Wire">The external Wire of the Face</returns>
		property Wire^ ExternalBoundary
		{
			Wire^ get();
		}

		/// <summary>
		/// Returns the internal boundaries (Wires) of the Face.
		/// </summary>
		/// <returns name="Wire[]">A list of the internal Wires of the Face</returns>
		property List<Wire^>^ InternalBoundaries
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Adds internal boundaries (Wires) to a Face.
		/// </summary>
		/// <param name="internalBoundaries">A list of internal Wires</param>
		/// <returns name="Face">The new Face</returns>
		Face^ AddInternalBoundaries(List<Wire^>^ internalBoundaries);

		/// <summary>
		/// Adds an Aperture to a Face.
		/// </summary>
		/// <param name="face">A Face</param>
		/// <param name="apertureDesign">The Aperture design to be added</param>
		/// <param name="numEdgeSamples">The number of sample points along the Aperture Edges</param>
		/// <returns name="Face">The new Face</returns>
		static Face^ AddAperture(Face^ face, Face^ apertureDesign, int numEdgeSamples);

		/// <summary>
		/// Creates a geometry from Face.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ Geometry_
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the type associated to Face.
		/// </summary>
		/// <returns>The type associated to Face</returns>
		static int Type();

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

		/// <summary>
		/// Create a face by a surface.
		/// </summary>
		/// <param name="surface">The surface</param>
		/// <returns name="Face">The created face</returns>
		static Face^ BySurface(Autodesk::DesignScript::Geometry::Surface^ surface);

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