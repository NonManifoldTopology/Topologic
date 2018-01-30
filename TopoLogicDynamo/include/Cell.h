#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Cell.h>

namespace TopoLogic
{
	public ref class Cell : Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="faces"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cell", "Solid" })]
		static Dictionary<String^, Object^>^ ByFaces(List<Object^>^ faces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="shell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cell", "Solid" })]
		static Dictionary<String^, Object^>^ BySolid(Autodesk::DesignScript::Geometry::Solid^ solid);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="shell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cell", "Solid" })]
		static Dictionary<String^, Object^>^ ByShell(Object^ shell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="faceIndices"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cell", "Solid" })]
		static Dictionary<String^, Object^>^ ByVerticesFaceIndices(List<Object^>^ vertices, List<List<int>^>^ faceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic CellComplexes", "Solids" })]
		static Dictionary<String^, Object^>^ CellComplex(Cell^ topoLogicCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ Faces(Cell^ topoLogicCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Wires", "PolyCurves" })]
		static Dictionary<String^, Object^>^ Wires(Cell^ topoLogicCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ Edges(Cell^ topoLogicCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="topoLogicCell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ Vertices(Cell^ topoLogicCell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Cells", "Solids" })]
		static Dictionary<String^, Object^>^ AdjacentCells(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell1"></param>
		/// <param name="cell2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Faces", "Surfaces" })]
		static Dictionary<String^, Object^>^ SharedFaces(Cell^ cell1, Cell^ cell2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell1"></param>
		/// <param name="cell2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Edges", "Curves" })]
		static Dictionary<String^, Object^>^ SharedEdges(Cell^ cell1, Cell^ cell2);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell1"></param>
		/// <param name="cell2"></param>
		/// <returns></returns>
		[MultiReturn(gcnew array<String^> { "TopoLogic Vertices", "Points" })]
		static Dictionary<String^, Object^>^ SharedVertices(Cell^ cell1, Cell^ cell2);

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
		Cell();
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Cell* m_pCoreCell;
	};
}