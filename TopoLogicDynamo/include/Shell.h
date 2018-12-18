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
		/// Get the cell bounded by the shell. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <returns name="Cell[]">The cell bounded by the shell</returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Get the constituent faces of the shell. 
		/// </summary>
		/// <returns name="Face[]">The constituent faces</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the constituent wires of the shell.
		/// </summary>
		/// <returns name="Wire[]">The constituent wires</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Get the constituent edges of the shell.
		/// </summary>
		/// <returns name="Edge[]">The constituent edges</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the constituent vertices of the shell.
		/// </summary>
		/// <returns name="Vertex[]">The constituent vertices</returns>
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
		/// <param name="tolerance"></param>
		/// <returns name="Shell">The created shell</returns>
		static Shell^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces, [DefaultArgument("0.001")] double tolerance);

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