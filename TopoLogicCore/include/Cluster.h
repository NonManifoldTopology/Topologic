#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>

namespace TopologicCore
{
	class CellComplex;
	class Cell;
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;

	class ClusterGUID
	{
	public:
		static std::string Get()
		{
			return std::string("7c498db6-f3e7-4722-be58-9720a4a9c2cc");
		}
	};

	class Cluster : public Topology
	{
	public:
		typedef std::shared_ptr<Cluster> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		//Cluster(const std::string& rkGuid = "");

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctCompound"></param>x
		Cluster(const TopoDS_Compound& rkOcctCompound, const std::string& rkGuid = "");

		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cluster> ByTopologies(const std::list<std::shared_ptr<Topology>>& rkTopologies);

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
		virtual TopoDS_Compound& GetOcctCompound();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Compound& GetOcctCompound() const;

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

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		//TOPOLOGIC_API Cluster::Ptr Flatten();

		/// <summary>
		/// 
		/// </summary>
		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkParentTopology"></param>
		/// <returns></returns>
		virtual bool IsManifold(TopologicCore::Topology const * const kpkParentTopology) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CLUSTER; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return ClusterGUID::Get();
		}

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
		TopoDS_Compound m_occtCompound;

		TopoDS_Builder m_occtBuilder;
	};
}
