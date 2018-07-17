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
		/// 
		/// </summary>
		/// <param name="faces"></param>
		/// <returns name="Cell"></returns>
		static Cell^ ByFaces(List<Face^>^ faces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="solid"></param>
		/// <returns name="Cell"></returns>
		static Cell^ BySolid(Autodesk::DesignScript::Geometry::Solid^ solid);

		/// <summary>
		/// 
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
		static Cell^ ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices);

		/// <summary>
		/// Creates a cell by lofting through a set of wires.
		/// </summary>
		/// <param name="wires">A set of wires</param>
		/// <returns name="Cell">The created cell</returns>
		static Cell^ ByLoft(List<Wire^>^ wires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="parentTopology"></param>
		/// <returns name="CellComplex[]"></returns>
		List<CellComplex^>^ CellComplexes(Topology^ parentTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Shell[]"></returns>
		List<Shell^>^ Shells();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Face[]"></returns>
		List<Face^>^ Faces();

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
		/// <param name="parentTopology"></param>
		/// <returns name="Cell[]"></returns>
		List<Cell^>^ AdjacentCells(Topology^ parentTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns name="Face[]"></returns>
		List<Face^>^ SharedFaces(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns name="Face[]"></returns>
		List<Edge^>^ SharedEdges(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="cell"></param>
		/// <returns name="Vertex[]"></returns>
		List<Vertex^>^ SharedVertices(Cell^ cell);

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Shell"></returns>
		Shell^ OuterBoundary();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Shell[]"></returns>
		List<Shell^>^ InnerBoundaries();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="double"></returns>
		double Volume();

		/// <summary>
		/// 
		/// </summary>
		/// <returns name="Vertex"></returns>
		Vertex^ CenterOfMass();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="vertex"></param>
		/// <returns name="bool"></returns>
		bool DoesContain(Vertex^ vertex);

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
		Cell(const std::shared_ptr<TopologicCore::Cell>& kpCoreCell);

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
		std::shared_ptr<TopologicCore::Cell>* m_pCoreCell;
	};
}