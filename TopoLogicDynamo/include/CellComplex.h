#pragma once

#include <Topology.h>

#include <TopologicCore/include/CellComplex.h>

namespace Topologic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Shell;
	ref class Cell;

	public ref class CellComplex : Topology
	{
	public:
		/// <summary>
		/// Create a cellComplex from a set of cells.
		/// </summary>
		/// <param name="cells"></param>
		/// <returns name="CellComplex"></returns>
		static CellComplex^ ByCells(System::Collections::Generic::IEnumerable<Cell^>^ cells);

		/// <summary>
		/// Create a cellComplex from the space enclosed by a set of faces. Parts of the faces which do not enclose any space will be discarded.
		/// </summary>
		/// <param name="faces"></param>
		/// <returns name="CellComplex"></returns>
		static CellComplex^ ByFaces(System::Collections::Generic::IEnumerable<Face^>^ faces);

		/// <summary>
		/// Get the cells constituent to the cellComplex.
		/// </summary>
		/// <returns name="Cell[]"></returns>
		property List<Cell^>^ Cells
		{
			List<Cell^>^ get();
		}

		/// <summary>
		/// Get the faces constituent to the cellComplex.
		/// </summary>
		/// <returns name="Face[]"></returns>
		property List<Face^>^ Faces
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the shells constituent to the cellComplex.
		/// </summary>
		/// <returns name="Shell[]"></returns>
		property List<Shell^>^ Shells
		{
			List<Shell^>^ get();
		}

		/// <summary>
		/// Get the wires constituent to the cellComplex.
		/// </summary>
		/// <returns name="Wire[]"></returns>
		property List<Wire^>^ Wires
		{
			List<Wire^>^ get();
		}

		/// <summary>
		/// Get the edges constituent to the cellComplex.
		/// </summary>
		/// <returns name="Edge[]"></returns>
		property List<Edge^>^ Edges
		{
			List<Edge^>^ get();
		}

		/// <summary>
		/// Get the vertices constituent to the cellComplex.
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		property List<Vertex^>^ Vertices
		{
			List<Vertex^>^ get();
		}


		/// <summary>
		/// Get the external boundary (cell) of the cellComplex.
		/// </summary>
		/// <returns name="Cell"></returns>
		property Cell^ ExternalBoundary
		{
			Cell^ get();
		}

		/// <summary>
		/// Get the internal boundaries (faces) of the cellComplex.
		/// </summary>
		/// <returns name="Face[]"></returns>
		property List<Face^>^ InternalBoundaries
		{
			List<Face^>^ get();
		}

		/// <summary>
		/// Get the non-manifold faces of the cellComplex.
		/// </summary>
		/// <returns name="Face[]"></returns>
		property List<Face^>^ NonManifoldFaces
		{
			List<Face^>^ get();
		}

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
		CellComplex(const std::shared_ptr<TopologicCore::CellComplex>& kpCoreCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopologicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopologicCore::CellComplex>* m_pCoreCellComplex;
	};
}