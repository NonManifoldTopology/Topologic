#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/CellComplex.h>

namespace TopoLogic
{
	public ref class CellComplex : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="cells"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic CellComplex", "Solids" })]
		static Dictionary<String^, Object^>^ ByCells(List<Object^>^ cells);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cells", "Solids" })]
		static Dictionary<String^, Object^>^ Cells(CellComplex^ cellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ Faces(CellComplex^ cellComplex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cellComplex"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cell", "Solid" })]
		static Dictionary<String^, Object^>^ BoundingCell(CellComplex^ cellComplex);

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

	protected:
		/// <summary>
		/// 
		/// </summary>
		CellComplex();
		virtual ~CellComplex();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::CellComplex* m_pCoreCellComplex;
	};
}