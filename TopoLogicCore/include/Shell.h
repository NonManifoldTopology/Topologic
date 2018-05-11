#pragma once

#include "Topology.h"

#include <list>
#include <vector>

#include <TopoDS_Shell.hxx>

namespace TopologicCore
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
		/// <param name="rkOcctLabel"></param>
		Shell(const TopoDS_Shell& rkOcctShell, const TDF_Label& rkOcctLabel = TDF_Label());

		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpParentTopology"></param>
		/// <param name="rCells"></param>
		/// <returns></returns>
		Topologic_API void Cells(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		Topologic_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		Topologic_API void Wires(std::list<std::shared_ptr<Wire>>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		Topologic_API void Faces(std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		Topologic_API void Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Topologic_API bool IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static Topologic_API std::shared_ptr<Shell> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <param name="rkFaceIndices"></param>
		/// <returns></returns>
		static Topologic_API std::shared_ptr<Shell> ByVerticesFaceIndices(const std::vector<std::shared_ptr<Vertex>>& rkVertices, const std::list<std::list<int>>& rkFaceIndices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWires"></param>
		/// <returns></returns>
		static Topologic_API std::shared_ptr<Shell> ByLoft(const std::list<std::shared_ptr<Wire>>& rkWires);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shape& GetOcctShape();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Shape& GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Shell& GetOcctShell();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Shell& GetOcctShell() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kIteration"></param>
		/// <param name="kNumUPanels"></param>
		/// <param name="kNumVPanels"></param>
		/// <param name="kTolerance"></param>
		/// <param name="kCapBottom"></param>
		/// <param name="kCapTop"></param>
		/// <returns></returns>
		Topologic_API static std::shared_ptr<Shell> ByFacePlanarization(
			const std::shared_ptr<Face>& kpFace,
			const int kIteration,
			const int kEdgeSamples,
			const int kNumUPanels,
			const int kNumVPanels,
			const double kTolerance,
			const bool kCapBottom,
			const bool kCapTop,
			std::list<std::shared_ptr<Vertex>>& vertices,
			std::list<std::shared_ptr<Edge>>& edges,
			std::list<std::shared_ptr<Wire>>& wires,
			std::list<std::shared_ptr<Face>>& faces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpFace"></param>
		/// <param name="kIteration"></param>
		/// <param name="rkUValues"></param>
		/// <param name="rkVValues"></param>
		/// <param name="kTolerance"></param>
		/// <param name="kCapBottom"></param>
		/// <param name="kCapTop"></param>
		/// <returns></returns>
		Topologic_API static std::shared_ptr<Shell> ByFacePlanarization(
			const std::shared_ptr<Face>& kpFace,
			const int kIteration,
			const int kEdgeSamples,
			const std::list<double>& rkUValues,
			const std::list<double>& rkVValues,
			const double kTolerance,
			const bool kCapBottom,
			const bool kCapTop,
			std::list<std::shared_ptr<Vertex>>& vertices,
			std::list<std::shared_ptr<Edge>>& edges,
			std::list<std::shared_ptr<Wire>>& wires,
			std::list<std::shared_ptr<Face>>& faces);


		virtual TopologyType GetType() const { return TOPOLOGY_SHELL; }

	protected:
		/// <summary>
		/// The underlying OCCT shell.
		/// </summary>
		TopoDS_Shell m_occtShell;
	};
}
