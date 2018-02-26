#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/CellComplex.h>

namespace TopoLogic
{
	ref class Face;
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
		Cell^ Envelope();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Face^>^ InternalFaces();

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
		virtual TopoLogicCore::Topology* GetCoreTopology() override;

	protected:
		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::CellComplex* m_pCoreCellComplex;
	};
}