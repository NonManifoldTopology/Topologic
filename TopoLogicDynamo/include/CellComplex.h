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
		/// <returns></returns>
		static CellComplex^ ByCells(List<Cell^>^ cells);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Cell^>^ Cells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Face^>^ Faces();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Shell^>^ Shells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Vertex^>^ Vertices();


		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Cell^ OuterBoundary();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Face^>^ InnerBoundaries();

		property bool IsClosed {
			/// <summary>
			/// 
			/// </summary>
			/// <returns></returns>
			bool get();
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
		CellComplex(TopoLogicCore::CellComplex* const kpCoreCellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::TopologicalQuery* GetCoreTopologicalQuery() override;

	protected:
		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::CellComplex* m_pCoreCellComplex;
	};
}