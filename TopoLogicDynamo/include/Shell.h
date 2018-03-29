#pragma once

#include <Topology.h>

#include <TopoLogicCore/include/Shell.h>

namespace TopoLogic
{
	ref class Vertex;
	ref class Edge;
	ref class Wire;
	ref class Face;
	ref class Cell;

	public ref class Shell : Topology
	{
	public:
		/// <summary>
		/// Gets the cell bounded by this shell.
		/// </summary>
		/// <returns>The cell bounded by this shell</returns>
		List<Cell^>^ Cells();

		/// <summary>
		/// Gets the list of faces constituent to this face. 
		/// </summary>
		/// <returns>The faces constituent to this face</returns>
		List<Face^>^ Faces();

		/// <summary>
		/// Gets the list of wires constituent to this shell.
		/// </summary>
		/// <returns>The wires consituent to this shell</returns>
		List<Wire^>^ Wires();

		/// <summary>
		/// Gets the list of edges constituent to this shell.
		/// </summary>
		/// <returns>The edges consituent to this shell</returns>
		List<Edge^>^ Edges();

		/// <summary>
		/// Gets the list of vertices constituent to this shell.
		/// </summary>
		/// <returns>The vertices consituent to this shell</returns>
		List<Vertex^>^ Vertices();

		/// <summary>
		/// Returns true if the shell is closed and false otherwise.
		/// </summary>
		/// <returns>True if the shell is closed and false otherwise</returns>
		property bool IsClosed {
			bool get();
		}

		/// <summary>
		/// Creates a shell by a set of connected faces.
		/// </summary>
		/// <param name="faces">A set of faces.</param>
		/// <returns>The created shell</returns>
		static Shell^ ByFaces(List<Face^>^ faces);

		/// <summary>
		/// Creates a shell by a polysurface.
		/// </summary>
		/// <param name="polysurface">A Dynamo polysurface</param>
		/// <returns>The created shell</returns>
		static Shell^ ByPolysurface(Autodesk::DesignScript::Geometry::PolySurface^ polysurface);

		/// <summary>
		/// Creates a shell by lofting through a set of wires.
		/// </summary>
		/// <param name="Wire[]">A set of wires</param>
		/// <returns>The created shell</returns>
		static Shell^ ByLoft(List<Wire^>^ wires);

		/// <summary>
		/// Creates a shell by a list of vertices and a list of face indices.
		/// </summary>
		/// <param name="vertices">A list of vertices.</param>
		/// <param name="faceIndices">A list of face indices. Every element must be within the range of the vertices argument.</param>
		/// <returns>The created shell</returns>
		static Shell^ ByVerticesFaceIndices(List<Vertex^>^ vertices, List<List<int>^>^ faceIndices);
		
		/// <summary>
		/// 
		/// </summary>
		/// <param name="face"></param>
		/// <param name="iteration"></param>
		/// <param name="numUPanels"></param>
		/// <param name="numVPanels"></param>
		/// <param name="tolerance"></param>
		/// <param name="isCapped">Cap the holes if the surface is closed either in the u- or v-direction but open on the other</param>
		/// <returns></returns>
		static Shell^ ByFacePlanarization(Face^ face, int iteration, int numUPanels, int numVPanels, double tolerance, bool isCapped);

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
		/// <param name="kpCoreShell"></param>
		Shell(const std::shared_ptr<TopoLogicCore::Shell>& kpCoreShell);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::shared_ptr<TopoLogicCore::TopologicalQuery> GetCoreTopologicalQuery() override;

	protected:
		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		std::shared_ptr<TopoLogicCore::Shell>* m_pCoreShell;
	};
}