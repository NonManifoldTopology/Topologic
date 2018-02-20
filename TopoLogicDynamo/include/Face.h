#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Face.h>

namespace TopoLogic {
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
		/// Gets the list of faces adjacent to this face.
		/// </summary>
		/// <returns>The faces adjacent to this face</returns>
		List<Face^>^ AdjacentFaces();

		/// <summary>
		/// Gets the list of aperture wires of this face.
		/// </summary>
		/// <returns>The apertures of this face</returns>
		List<Face^>^ Apertures();

		/// <summary>
		/// Gets the list of cells incident to this face.
		/// </summary>
		/// <returns>The cells incident to this face</returns>
		List<Cell^>^ Cells();

		/// <summary>
		/// Returns true if the apertures are applied and false otherwise.
		/// </summary>
		/// <returns>True if the apertures are applied and false otherwise</returns>
		property bool IsApplied {
			bool get();
		}

		/// <summary>
		/// Gets the list of shells incident to this face.
		/// </summary>
		/// <returns>The shells incident to this face</returns>
		List<Shell^>^ Shells();

		/// <summary>
		/// Gets the list of vertices constituent to this face. 
		/// </summary>
		/// <returns>The vertices consituent to this face</returns>
		List<Vertex^>^ Vertices();

		/// <summary>
		/// Gets the list of edges constituent to this face.
		/// </summary>
		/// <returns>The edges consituent to this face</returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// Gets the list of wires constituent to this face.
		/// </summary>
		/// <returns>The wires consituent to this face</returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// Call this method to apply apertures.
		/// </summary>
		/// <param name="apertures"></param>
		/// <returns></returns>
		Face^ ApplyApertures(List<Face^>^ apertures);

		/// <summary>
		/// A factory method that creates a face by a closed wire.
		/// </summary>
		/// <param name="wire">A closed wire. Must be (and internally verified if it is) a Dynamo polygon or a TopoLogic wire.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is neither a Dynamo polygon nor a TopoLogic wire</exception>
		/// <returns name="TopoLogic Face">
		/// "TopoLogic Face": the created face
		/// </returns>
		/// <returns name="Surface">
		/// "Surface": the Dynamo surface counterpart of the created face
		/// </returns>
		static Face^ ByClosedWire(Wire^ wire);

		/// <summary>
		/// Creates a face by a list of edges. A wire will be internally created.
		/// </summary>
		/// <param name="edges">The edges. </param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is not a TopoLogic edge</exception>
		/// <returns>the created face</returns>
		static Face^ ByEdges(List<Edge^>^ edges);

		/// <summary>
		/// Creates a face by a surface.
		/// </summary>
		/// <param name="surface">The surface</param>
		/// <returns>The created face</returns>
		static Face^ BySurface(Autodesk::DesignScript::Geometry::Surface^ surface);

		/// <summary>
		/// Returns the shared edges between two faces. 
		/// </summary>
		/// <param name="face">Another face</param>
		/// <returns>The shared edges</returns>
		List<Edge^>^ SharedEdges(Face^ face);

		/// <summary>
		/// Returns the shared vertices between two faces.
		/// </summary>
		/// <param name="face">Another face</param>
		/// <returns>The shared vertices</returns>
		List<Vertex^>^ SharedVertices(Face^ face);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Wire^ OuterWire();

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
		Face(TopoLogicCore::Face* const kpCoreFace);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pWire"></param>
		Face(Wire^ pWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pDynamoSurface"></param>
		Face(Autodesk::DesignScript::Geometry::Surface^ pDynamoSurface);

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
		virtual TopoLogicCore::Topology* GetCoreTopology() override;

	protected:
		virtual ~Face();

		/// <summary>
		/// Initialises the face given a NurbsSurface argument. Called by the respective constructor.
		/// </summary>
		/// <param name="pDynamoNurbsSurface">A Dynamo NURBS surface</param>
		/// <exception cref="Standard_ConstructionError">Thrown if OCCT fails to initialise the underlying curve</exception>
		/// <exception cref="StdFail_NotDone">Thrown if OCCT fails to create an edge from the curve</exception>
		void Init(Autodesk::DesignScript::Geometry::NurbsSurface^ pDynamoNurbsSurface,
			array<Autodesk::DesignScript::Geometry::Curve^>^ pDynamoPerimeterCurves);

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Face* m_pCoreFace;
	};
}