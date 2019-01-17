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
		/// Create a cell by a set of faces.
		/// </summary>
		/// <param name="faces"></param>
		/// <returns name="Cell"></returns>
		static Cell^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces);

		/// <summary>
		/// Create a cell from a shell. The shell must be closed, otherwise an exception is thrown.
		/// </summary>
		/// <param name="shell"></param>
		/// <returns name="Cell"></returns>
		static Cell^ ByShell(Shell^ shell);

		/// <summary>
		/// Get the cellComplexes which contain the cell.
		/// </summary>
		/// <returns name="CellComplex[]"></returns>
		property List<CellComplex^>^ CellComplexes
		{
			List<CellComplex^>^ get();
		}

		/// <summary>
		/// Get the shells constituent to the cell.
		/// </summary>
		/// <returns name="Shell[]"></returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Get the faces constituent to the cell.
		/// </summary>
		/// <returns name="Face[]"></returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the wires constituent to the cell.
		/// </summary>
		/// <returns name="Wire[]"></returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Get the edges constituent to the cell.
		/// </summary>
		/// <returns name="Edge[]"></returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the vertices constituent to the cell.
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}

		/// <summary>
		/// Get the cells adjacent to the cell.
		/// </summary>
		/// <returns name="Cell[]"></returns>
		/// 
		property List<Cell^>^ AdjacentCells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Return the shared faces between two cells. 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns name="Face[]"></returns>
		List<Face^>^ SharedFaces(Cell^ cell);

		/// <summary>
		/// Return the shared edges between two cells. 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns name="Face[]"></returns>
		List<Edge^>^ SharedEdges(Cell^ cell);

		/// <summary>
		/// Return the shared vertices between two cells. 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns name="Vertex[]"></returns>
		List<Vertex^>^ SharedVertices(Cell^ cell);

		/// <summary>
		/// Return the external boundary (shell) of the cell.
		/// </summary>
		/// <returns name="Shell"></returns>
		property Shell^ ExternalBoundary
		{
			Shell^ get();
		}

		/// <summary>
		/// Return the internal boundary (shells) of the cell.
		/// </summary>
		/// <returns name="Shell[]"></returns>
		property List<Shell^>^ InternalBoundaries
		{
			List<Shell^>^ get();
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
		/// <param name="kpCoreCell"></param>
		Cell(const std::shared_ptr<TopologicCore::Cell>& kpCoreCell);

		/// <summary>
		/// Create a cell by a solid.
		/// </summary>
		/// <param name="solid"></param>
		/// <returns name="Cell"></returns>
		static Cell^ BySolid(Autodesk::DesignScript::Geometry::Solid^ solid);

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