#pragma once

#include "Topology.h"

#include <list>
#include <vector>

#include <TopoDS_Shell.hxx>

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class Shell : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShell"></param>
		Shell(const TopoDS_Shell& rkOcctShell);

		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Cells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<std::shared_ptr<Wire>>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void Faces(std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API bool IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Shell> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Shell> ByVerticesFaceIndices(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		virtual std::shared_ptr<TopoDS_Shape> GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kIteration"></param>
		/// <param name="kNumUPanels"></param>
		/// <param name="kNumVPanels"></param>
		/// <param name="kTolerance"></param>
		/// <returns></returns>
		TOPOLOGIC_API static std::shared_ptr<Shell> ByFacePlanarization(
			const std::shared_ptr<Face>& kpFace,
			const int kIteration,
			const int kNumUPanels,
			const int kNumVPanels,
			const double kTolerance);

		virtual TopologyType GetType() const { return TOPOLOGY_SHELL; }

	protected:
		/// <summary>
		/// The underlying OCCT shell.
		/// </summary>
		std::shared_ptr<TopoDS_Shell> m_pOcctShell;
	};
}
