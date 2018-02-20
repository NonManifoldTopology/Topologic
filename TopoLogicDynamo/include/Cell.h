#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Cell.h>

namespace TopoLogic
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
		/// 
		/// </summary>
		/// <returns></returns>
		static Cell^ ByFaces(List<Face^>^ faces);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		static Cell^ BySolid(Autodesk::DesignScript::Geometry::Solid^ solid);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="shell"></param>
		/// <returns></returns>
		static Cell^ ByShell(Shell^ shell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertices"></param>
		/// <param name="faceIndices"></param>
		/// <returns></returns>
		static Cell^ ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		CellComplex^ CellComplex();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Shell^>^ Shells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		List<Face^>^ Faces();

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
		/// <param name="cell"></param>
		/// <returns></returns>
		List<Cell^>^ AdjacentCells(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns></returns>
		List<Face^>^ SharedFaces(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns></returns>
		List<Edge^>^ SharedEdges(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns></returns>
		List<Vertex^>^ SharedVertices(Cell^ cell);

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
		/// <param name="solid"></param>
		Cell(Autodesk::DesignScript::Geometry::Solid^ solid);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="faces"></param>
		Cell::Cell(List<Face^>^ faces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCoreCell"></param>
		Cell(TopoLogicCore::Cell* const kpCoreCell);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoLogicCore::Topology* GetCoreTopology() override;

	protected:
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="faces"></param>
		void Init(List<Face^>^ faces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cuboid"></param>
		void Init(Autodesk::DesignScript::Geometry::Cuboid^ cuboid);

		/// <summary>
		/// 
		/// </summary>
		TopoLogicCore::Cell* m_pCoreCell;
	};
}