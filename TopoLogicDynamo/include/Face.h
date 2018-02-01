#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Face.h>

namespace TopoLogic {
	ref class Edge;
	ref class Wire;

	/// <summary>
	/// 
	/// </summary>
	public ref class Face : Topology
	{
	public:
		/// <summary>
		/// Gets the list of faces adjacent to this face.
		/// </summary>
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Faces">
		/// "TopoLogic Faces": the faces adjacent to this face
		/// </returns>
		/// <returns name="Surfaces">
		/// "Surfaces": the Dynamo surfaces counterpart of the adjacent faces
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ AdjacentFaces(Face^ topoLogicFace);

		/// <summary>
		/// Gets the list of aperture wires of this face.
		/// </summary>
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Wires">
		/// "TopoLogic Wires": the aperture wires of this face
		/// </returns>
		/// <returns name="PolyCurves">
		/// "PolyCurves": the Dynamo polycurves counterpart of the aperture wires
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wires", "PolyCurves" })]
		static Dictionary<String^, Object^>^ Apertures(Face^ topoLogicFace);

		/// <summary>
		/// Gets the list of cells incident to this face.
		/// </summary>
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Cells">
		/// "TopoLogic Cells": the cells incident to this face
		/// </returns>
		/// <returns name="Solids">
		/// "Solids": the Dynamo solids counterpart of the incident cells
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cells", "Solids" })]
		static Dictionary<String^, Object^>^ Cells(Face^ topoLogicFace);

		/// <summary>
		/// Gets the list of edges constituent to this face. 
		/// </summary>
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Edges">
		/// "TopoLogic Edges": the edges consituent to this face
		/// </returns>
		/// <returns name="Curves">
		/// "Curves": the Dynamo curves counterpart of the constituent edges
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ Edges(Face^ topoLogicFace);

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
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Shells">
		/// "TopoLogic Shells": the shells incident to this face
		/// </returns>
		/// <returns name="Meshes">
		/// "PolySurfaces": the Dynamo polysurfaces counterpart of the incident shells
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Shells", "Meshes" })]
		static Dictionary<String^, Object^>^ Shells(Face^ topoLogicFace);

		/// <summary>
		/// Gets the list of vertices constituent to this face. 
		/// </summary>
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Vertices">
		/// "TopoLogic Vertices": the vertices consituent to this face
		/// </returns>
		/// <returns name="Points">
		/// "Points": the Dynamo points counterpart of the constituent points
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ Vertices(Face^ topoLogicFace);

		/// <summary>
		/// Gets the list of wires constituent to this face.
		/// </summary>
		/// <param name="topoLogicFace">A face</param>
		/// <returns name="TopoLogic Wires">
		/// "TopoLogic Wires": the wires consituent to this face
		/// </returns>
		/// <returns name="PolyCurves">
		/// "PolyCurves": the Dynamo polycurves counterpart of the constituent wires
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wires", "PolyCurves" })]
		static Dictionary<String^, Object^>^ Wires(Face^ topoLogicFace);

		/// <summary>
		/// Call this method to apply apertures.
		/// </summary>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Face", "Aperture Faces", "Surface", "Aperture Surfaces" })]
		static Dictionary<String^, Object^>^ ApplyApertures(Face^ topoLogicFace, List<Face^>^ apertures);

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
		[MultiReturn(gcnew array<String^> { "TopoLogic Face", "Surface" })]
		static Dictionary<String^, Object^>^ ByClosedWire(Wire^ wire);

		/// <summary>
		/// A factory method that directly creates a face by a list of edges. A wire will be internally created.
		/// </summary>
		/// <param name="edges">The edges. Must be (and internally verified if it is) a list of Dynamo or TopoLogic edges.</param>
		/// <exception cref="ArgumentException">Thrown if any of the arguments is neither a list of Dynamo nor TopoLogic edges</exception>
		/// <returns name="TopoLogic Face">
		/// "TopoLogic Face": the created face
		/// </returns>
		/// <returns name="Surface">
		/// "Surface": the Dynamo surface counterpart of the created face
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Face", "Surface" })]
		static Dictionary<String^, Object^>^ ByEdges(List<Edge^>^ edges);

		/// <summary>
		/// A factory method that creates a face by a surface
		/// </summary>
		/// <param name="surface">The surface</param>
		/// <returns name="TopoLogic Face">
		/// "TopoLogic Face": the created face
		/// </returns>
		/// <returns name="Surface">
		/// "Surface": the Dynamo surface counterpart of the created face
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Face", "Surface" })]
		static Dictionary<String^, Object^>^ BySurface(Autodesk::DesignScript::Geometry::Surface^ surface);

		/// <summary>
		/// Returns the shared edges between two faces. 
		/// </summary>
		/// <param name="face1">A face</param>
		/// <param name="face2">Another face</param>
		/// <returns name="TopoLogic Edges">
		/// "TopoLogic Edges": the shared edges
		/// </returns>
		/// <returns name="Curves">
		/// "Curves": the Dynamo curves counterpart of the shared edges
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ SharedEdges(Face^ face1, Face^ face2);

		/// <summary>
		/// Returns the shared vertices between two faces. 
		/// </summary>
		/// <param name="face1">A face</param>
		/// <param name="face2">Another face</param>
		/// <returns name="TopoLogic Vertices">
		/// "TopoLogic Vertices": the shared vertices
		/// </returns>
		/// <returns name="Points">
		/// "Points": the Dynamo points counterpart of the shared edges
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ SharedVertices(Face^ face1, Face^ face2);

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