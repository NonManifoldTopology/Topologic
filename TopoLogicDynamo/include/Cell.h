#pragma once

#include <Topology.h>

#include <TopologicCore/include/Cell.h>

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Shell;
	ref class CellComplex;

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
		/// Create a cell by a solid. NOTE: This node will be replaced by a single Topology.ByGeometry() node.
		/// </summary>
		/// <param name="solid"></param>
		/// <returns name="Cell"></returns>
		static Cell^ BySolid_(Autodesk::DesignScript::Geometry::Solid^ solid);

		/// <summary>
		/// Create a cell from a shell. The shell must be closed, otherwise an exception is thrown.
		/// </summary>
		/// <param name="shell"></param>
		/// <returns name="Cell"></returns>
		static Cell^ ByShell(Shell^ shell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="faceIndices"></param>
		/// <returns name="Cell"></returns>
		[IsVisibleInDynamoLibrary(false)]
		static Cell^ ByVerticesFaceIndices(System::Collections::Generic::IEnumerable<Vertex^>^ vertices, System::Collections::Generic::IEnumerable<System::Collections::Generic::IEnumerable<int>^>^ faceIndices);

		/// <summary>
		/// Creates a cell by lofting through a set of wires.
		/// </summary>
		/// <param name="wires">A set of wires</param>
		/// <returns name="Cell">The created cell</returns>
		[IsVisibleInDynamoLibrary(false)]
		static Cell^ ByLoft(System::Collections::Generic::IEnumerable<Wire^>^ wires);

		/// <summary>
		/// Get the cellComplexes which contain the cell. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="CellComplex[]"></returns>
		List<CellComplex^>^ CellComplexes_(Topology^ hostTopology);

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
		/// Get the cells adjacent to the cell. NOTE: This instance method will be converted to a property, and the hostTopology argument will be removed.
		/// </summary>
		/// <param name="hostTopology"></param>
		/// <returns name="Cell[]"></returns>
		List<Cell^>^ AdjacentCells_(Topology^ hostTopology);

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

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="double"></returns>
		[IsVisibleInDynamoLibrary(false)]
		double Volume();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns name="bool"></returns>
		[IsVisibleInDynamoLibrary(false)]
		bool Contains(Vertex^ vertex);

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
		/// <param name="kpCoreCell"></param>
		Cell(const std::shared_ptr<TopologicCore::Cell>& kpCoreCell);

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