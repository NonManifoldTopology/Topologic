#pragma once

#include <TopologicCore/include/Cell.h>

#include "Topology.h"

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Shell;
	ref class CellComplex;

	/// <summary>
	/// A Cell is a three-dimensional region defined by a collection of closed Shells. It may be manifold or non-manifold.
	/// </summary>

	public ref class Cell : Topology
	{
	public:
		/// <summary>
		/// Creates a Cell by a set of Faces.
		/// </summary>
		/// <param name="faces">A set of Faces</param>
		/// <param name="tolerance">A tolerance value</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces, [DefaultArgument("0.001")] double tolerance);

		/// <summary>
		/// Creates a Cell from a Shell. The Shell must be closed, otherwise an exception is thrown.
		/// </summary>
		/// <param name="shell">A Shell</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ ByShell(Shell^ shell);

		/// <summary>
		/// Returns the CellComplexes which contain the Cell.
		/// </summary>
		/// <returns name="CellComplex[]">A list of CellComplexes containing the Cell</returns>
		property List<CellComplex^>^ CellComplexes
		{
			List<CellComplex^>^ get();
		}

		/// <summary>
		/// Returns the Shells constituent to the Cell.
		/// </summary>
		/// <returns name="Shell[]">A list of Shells constituent to the Cell</returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Returns the Faces constituent to the Cell.
		/// </summary>
		/// <returns name="Face[]">A list of Faces constituent to the Cell</returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Returns the Wires constituent to the Cell.
		/// </summary>
		/// <returns name="Wire[]">A list of Wires constituent to the Cell</returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Returns the Edges constituent to the Cell.
		/// </summary>
		/// <returns name="Edge[]">A list of Edges constituent to the Cell</returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Returns the Vertices constituent to the Cell.
		/// </summary>
		/// <returns name="Vertex[]">A list of Vertices constituent to the Cell</returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Returns the Cells adjacent to the Cell.
		/// </summary>
		/// <returns name="Cell[]">A list of Cells adjacent to the Cell</returns>
		/// 
		property List<Cell^>^ AdjacentCells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Returns the shared Faces between two Cells. 
		/// </summary>
		/// <param name="cell">Another Cell</param>
		/// <returns name="Face[]">A list of shared Faces between two Cells</returns>
		List<Face^>^ SharedFaces(Cell^ cell);

		/// <summary>
		/// Returns the shared Edges between two Cells. 
		/// </summary>
		/// <param name="cell">Another Cell</param>
		/// <returns name="Edge[]">A list of shared Edges between two Cells</returns>
		List<Edge^>^ SharedEdges(Cell^ cell);

		/// <summary>
		/// Returns the shared Vertices between two Cells. 
		/// </summary>
		/// <param name="cell">Another Cell</param>
		/// <returns name="Vertex[]">A list of shared Vertices between two Cells</returns>
		List<Vertex^>^ SharedVertices(Cell^ cell);

		/// <summary>
		/// Returns the external boundary (Shell) of the Cell.
		/// </summary>
		/// <returns name="Shell">The external boundary (Shell) of the Cell</returns>
		property Shell^ ExternalBoundary
		{
			Shell^ get();
		}

		/// <summary>
		/// Returns the internal boundaries (Shells) of the Cell.
		/// </summary>
		/// <returns name="Shell[]">A list of the internal boundaries (Shells) of the Cell</returns>
		property List<Shell^>^ InternalBoundaries
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Creates a geometry from Cell.
		/// </summary>
		/// <returns>The created geometry</returns>
		property Object^ Geometry_
		{
			virtual Object^ get() override;
		}

		/// <summary>
		/// Returns the type associated to Cell.
		/// </summary>
		/// <returns>The type associated to Cell</returns>
		static int Type();

	public protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreCell"></param>
		Cell(const std::shared_ptr<TopologicCore::Cell>& kpCoreCell);

		/// <summary>
		/// Creates a cell from a solid.
		/// </summary>
		/// <param name="solid">A solid</param>
		/// <param name="tolerance">A tolerance value</param>
		/// <returns name="Cell">The created Cell</returns>
		static Cell^ BySolid(Autodesk::DesignScript::Geometry::Solid^ solid, [DefaultArgument("0.001")] double tolerance);

		static Cell^ BySphere(Autodesk::DesignScript::Geometry::Sphere^ sphere);

		static Cell^ ByCylinder(Autodesk::DesignScript::Geometry::Cylinder^ cylinder);

		static Cell^ ByCone(Autodesk::DesignScript::Geometry::Cone^ cone);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cuboid"></param>
		static Cell^ ByCuboid(Autodesk::DesignScript::Geometry::Cuboid^ cuboid);

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::Cell>* m_pCoreCell;
	};
}