#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>

namespace TopoLogicCore
{
	class CellComplex;
	class Cell;
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;

	class Cluster : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kAddToGlobalCluster"></param>
		Cluster(const bool kAddToGlobalCluster = true);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctCompound"></param>
		/// <param name="kAddToGlobalCluster"></param>
		Cluster(const TopoDS_Compound& rkOcctCompound, const bool kAddToGlobalCluster = true);

		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cluster> ByTopology(const std::list<std::shared_ptr<Topology>>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpTopology"></param>
		/// <param name="kCheckIfInside"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool AddTopology(Topology const * const kpkTopology, const bool kCheckIfInside = false);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool RemoveTopology(Topology const * const kpkTopologyy);

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
		/// <param name="rShells"></param>
		TOPOLOGIC_API void Shells(std::list<std::shared_ptr<Shell>>& rShells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

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
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<std::shared_ptr<Wire>>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Cells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void CellComplexes(std::list<std::shared_ptr<CellComplex>>& rCellComplexes) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CLUSTER; }

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		bool IsInside(Topology const * const kpkTopology) const;

		/// <summary>
		/// The underlying OCCT compound.
		/// </summary>
		std::shared_ptr<TopoDS_Compound> m_pOcctCompound;

		TopoDS_Builder m_occtBuilder;
	};
}
