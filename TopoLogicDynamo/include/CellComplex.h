#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/CellComplex.h>

namespace TopoLogic
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
		static CellComplex^ ByCells(List<Cell^>^ cells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="faces"></param>
		/// <returns name="CellComplex"></returns>
		static CellComplex^ ByFaces(List<Face^>^ faces);

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
		CellComplex(const std::shared_ptr<TopoLogicCore::CellComplex>& kpCoreCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopoLogicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopoLogicCore::CellComplex>* m_pCoreCellComplex;
	};
}