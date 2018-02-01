#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Shell.h>

namespace TopoLogic
{
	public ref class Shell : Topology
	{
	public:
		/// <summary>
		/// Gets the cell bounded by this shell.
		/// </summary>
		/// <param name="topoLogicShell">A shell</param>
		/// <returns name="TopoLogic Cells">
		/// "TopoLogic Cell": the cell bounded by this shell
		/// </returns>
		/// <returns name="Solid">
		/// "Solid": the Dynamo solid counterpart of the bounded cell
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cell", "Solid" })]
		static Dictionary<String^, Object^>^ Cell(Shell^ topoLogicShell);

		/// <summary>
		/// Gets the list of faces constituent to this face. 
		/// </summary>
		/// <param name="topoLogicShell">A shell</param>
		/// <returns name="TopoLogic Faces">
		/// "TopoLogic Faces": the faces constituent to this face
		/// </returns>
		/// <returns name="Surfaces">
		/// "Surfaces": the Dynamo surfaces counterpart of the constituent faces
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ Faces(Shell^ topoLogicShell);

		/// <summary>
		/// Gets the list of edges constituent to this shell. 
		/// </summary>
		/// <param name="topoLogicShell">A shell</param>
		/// <returns name="TopoLogic Edges">
		/// "TopoLogic Edges": the edges consituent to this shell
		/// </returns>
		/// <returns name="Curves">
		/// "Curves": the Dynamo curves counterpart of the constituent edges
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ Edges(Shell^ topoLogicShell);

		/// <summary>
		/// Gets the list of vertices constituent to this shell.
		/// </summary>
		/// <param name="topoLogicShell">A shell</param>
		/// <returns name="TopoLogic Vertices">
		/// "TopoLogic Vertices": the vertices consituent to this shell
		/// </returns>
		/// <returns name="Points">
		/// "Points": the Dynamo points counterpart of the constituent points
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ Vertices(Shell^ topoLogicShell);

		/// <summary>
		/// Returns true if the shell is closed and false otherwise.
		/// </summary>
		/// <returns>True if the shell is closed and false otherwise</returns>
		property bool IsClosed {
			bool get();
		}

		/// <summary>
		/// Creates a shell by a set of connected faces.
		/// </summary>
		/// <param name="faces">A set of faces. Must be (and internally verified if it is) a list of Dynamo or TopoLogic faces.</param>
		/// <returns name="TopoLogic Shell">
		/// "TopoLogic Shell": the created shell
		/// </returns>
		/// <returns name="PolySurface">
		/// "PolySurface": the Dynamo polysurface counterpart of the created sje;;
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Shell", "PolySurface" })]
		static Dictionary<String^, Object^>^ ByFaces(List<Object^>^ faces);

		/// <summary>
		/// Creates a shell by a polysurface.
		/// </summary>
		/// <param name="polysurface">A Dynamo polysurface</param>
		/// <returns name="TopoLogic Shell">
		/// "TopoLogic Shell": the created shell
		/// </returns>
		/// <returns name="PolySurface">
		/// "PolySurface": the Dynamo polysurface counterpart of the created sje;;
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Shell", "PolySurface" })]
		static Dictionary<String^, Object^>^ ByPolysurface(Autodesk::DesignScript::Geometry::PolySurface^ polysurface);

		/// <summary>
		/// Creates a shell by a list of vertices and a list of face indices.
		/// </summary>
		/// <param name="vertices">A list of vertices. Must be (and internally verified if it is) a list of Dynamo or TopoLogic vertices.</param>
		/// <param name="faceIndices">A list of face indices. Every element must be within the range of the vertices argument.</param>
		/// <returns name="TopoLogic Shell">
		/// "TopoLogic Shell": the created shell
		/// </returns>
		/// <returns name="PolySurface">
		/// "PolySurface": the Dynamo polysurface counterpart of the created sje;;
		/// </returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Shell", "PolySurface" })]
		static Dictionary<String^, Object^>^ ByVerticesFaceIndices(List<Object^>^ vertices, List<List<int>^>^ faceIndices);

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
		/// <param name="kpCoreShell"></param>
		Shell(TopoLogicCore::Shell* const kpCoreShell);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::Topology* GetCoreTopology() override;


	protected:
		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Shell* m_pCoreShell;
	};
}