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
		TOPOLOGIC_API void Cells(std::list<Cell*>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<Wire*>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void Faces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void Vertices(std::list<Vertex*>& rVertices) const;

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
		static TOPOLOGIC_API Shell* ByFaces(const std::list<Face*>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Shell* ByVerticesFaceIndices(const std::vector<Vertex*>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFace"></param>
		/// <param name="kIteration"></param>
		/// <param name="kNumUPanels"></param>
		/// <param name="kNumVPanels"></param>
		/// <param name="kTolerance"></param>
		/// <returns></returns>
		TOPOLOGIC_API static Shell* ByFacePlanarization(
			Face& rFace,
			const int kIteration,
			const int kNumUPanels,
			const int kNumVPanels,
			const double kTolerance);

		virtual TopologyType GetType() const { return TOPOLOGY_SHELL; }

	protected:
		/// <summary>
		/// The underlying OCCT shell.
		/// </summary>
		TopoDS_Shell* m_pOcctShell;
	};
}
