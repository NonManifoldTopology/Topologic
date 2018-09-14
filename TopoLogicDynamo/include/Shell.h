#pragma once

#include <Topology.h>

#include <TopologicCore/include/Shell.h>

using namespace Autodesk::DesignScript::Runtime;
namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Cell;

	public ref class Shell : Topology
	{
	public:
		/// <summary>
		/// Get the cell bounded by the shell.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Cell[]">The cell bounded by the shell</returns>
		List<Cell^>^ Cells_(Topology^ hostTopology);

		/// <summary>
		/// Get the faces constituent to the shell. 
		/// </summary>
		/// <returns name="Face[]">The faces constituent to the face</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the wires constituent to the shell.
		/// </summary>
		/// <returns name="Wire[]">The wires consituent to the shell</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Get the edges constituent to the shell.
		/// </summary>
		/// <returns name="Edge[]">The edges consituent to the shell</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the vertices constituent to the shell.
		/// </summary>
		/// <returns name="Vertex[]">The vertices consituent to the shell</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Check if the shell is closed.
		/// </summary>
		/// <returns name="bool">True if the shell is closed and false otherwise</returns>
		property bool IsClosed {
			bool get();
		}

		/// <summary>
		/// Create a shell by a set of connected faces.
		/// </summary>
		/// <param name="faces">A set of faces.</param>
		/// <returns name="Shell">The created shell</returns>
		static Shell^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces);

		/// <summary>
		/// Create a shell by a polySurface.
		/// </summary>
		/// <param name="polySurface">A Dynamo polySurface</param>
		/// <returns name="Shell">The created shell</returns>
		static Shell^ ByPolySurface(Autodesk::DesignScript::Geometry::PolySurface^ polySurface);

		/// <summary>
		/// Create a shell by lofting through a set of wires.
		/// </summary>
		/// <param name="wires">A set of wires</param>
		/// <returns name="Shell">The created shell</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Shell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

		/// <summary>
		/// Create a shell by a list of vertices and a list of face indices.
		/// </summary>
		/// <param name="vertices">A list of vertices.</param>
		/// <param name="faceIndices">A list of face indices. Every element must be within the range of the vertices argument.</param>
		/// <returns name="Shell">The created shell</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Shell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);
		
		/// <summary>
		/// Create a shell by subdividing and planarizing a curved face.
		/// </summary>
		/// <param name="face"></param>
		/// <param name="iteration"></param>
		/// <param name="numUPanels"></param>
		/// <param name="numVPanels"></param>
		/// <param name="tolerance"></param>
		/// <param name="capBottom">Cap the holes if the surface is closed either in the minimum u- or v-direction but open on the other</param>
		/// <param name="capTop">Cap the holes if the surface is closed either in the maximum u- or v-direction but open on the other</param>
		/// <returns name="Face[]"></returns>
		[IsVisibleInDynamoLibrary(false)]
		static List<Face^>^ ByFacePlanarization_(Face^ face, int iteration, int numEdgeSamples, int numUPanels, int numVPanels, double tolerance, bool capBottom, bool capTop);

		/// <summary>
		/// Create a shell by subdividing and planarizing a curved face.
		/// </summary>
		/// <param name="face"></param>
		/// <param name="iteration"></param>
		/// <param name="uValues"></param>
		/// <param name="vValues"></param>
		/// <param name="tolerance"></param>
		/// <param name="capBottom">Cap the holes if the surface is closed either in the minimum u- or v-direction but open on the other</param>
		/// <param name="capTop">Cap the holes if the surface is closed either in the maximum u- or v-direction but open on the other</param>
		/// <returns></returns>
		//[MultiReturn(gcnew array<String^> { "Vertices", "Edges", "Wires", "Faces", "Shell" })]
		//static Dictionary<String^, Object^>^ ByFacePlanarization(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop);
		static Shell^ ByFacePlanarization_(Face^ face, int iteration, int numEdgeSamples, System::Collections::Generic::IEnumerable<double>^ uValues, System::Collections::Generic::IEnumerable<double>^ vValues, double tolerance, bool capBottom, bool capTop);

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
		Shell(const std::shared_ptr<TopologicCore::Shell>& kpCoreShell);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Shell>* m_pCoreShell;
	};
}