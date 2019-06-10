#pragma once

#include <Topology.h>

#include <TopologicCore/include/Shell.h>

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Cell;

	/// <summary>
	/// A Shell is a contiguous collection of Faces, where adjacent Faces are connected by shared Edges. It may be open or closed and may be manifold or non-manifold. 
	/// </summary>

	public ref class Shell : Topology
	{
	public:
		/// <summary>
		/// Returns the Cells bounded by the Shell.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells bounded by the Shell</returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Returns the Faces constituent to the Shell. 
		/// </summary>
		/// <returns name="Face[]">A list of the constituent Faces</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the Shell.
		/// </summary>
		/// <returns name="Wire[]">A list of the constituent Wires</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the Shell.
		/// </summary>
		/// <returns name="Edge[]">A list of the constituent Edges</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the Shell.
		/// </summary>
		/// <returns name="Vertex[]">A list of the constituent Vertices</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Checks if the Shell is closed.
		/// </summary>
		/// <returns name="bool">True if the Shell is closed, otherwise false</returns>
		property bool IsClosed {
			bool get();
		}

		/// <summary>
		/// Creates a Shell by a set of connected Faces.
		/// </summary>
		/// <param name="faces">A set of Faces</param>
		/// <param name="tolerance">A tolerance value</param>
		/// <returns name="Shell">The created Shell</returns>
#ifdef TOPOLOGIC_DYNAMO
		static Shell^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces, [DefaultArgument("0.001")] double tolerance);
#else
		static Shell^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces, double tolerance);
#endif

		/// <summary>
		/// Creates a geometry from Shell.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ BasicGeometry
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the type associated to Shell.
		/// </summary>
		/// <returns>The type associated to Shell</returns>
		static int Type();

	public protected:
		Shell();

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