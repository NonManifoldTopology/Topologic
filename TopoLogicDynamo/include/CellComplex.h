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
		/// 
		/// </summary>
		/// <param name="cells"></param>
		/// <returns name="CellComplex"></returns>
		static CellComplex^ ByCells(IEnumerable<Cell^>^ cells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="faces"></param>
		/// <returns name="CellComplex"></returns>
		static CellComplex^ ByFaces(IEnumerable<Face^>^ faces);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Cell[]"></returns>
		List<Cell^>^ Cells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Face[]"></returns>
		List<Face^>^ Faces();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Shell[]"></returns>
		List<Shell^>^ Shells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Wire[]"></returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Edge[]"></returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Vertex[]"></returns>
		List<Vertex^>^ Vertices();


		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Cell"></returns>
		Cell^ OuterBoundary();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Face[]"></returns>
		List<Face^>^ InnerBoundaries();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Face[]"></returns>
		List<Face^>^ NonManifoldFaces();

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