#pragma once

#include "Topology.h"

#include <list>
#include <vector>

#include <Geom_CartesianPoint.hxx>
#include <TopoDS_Solid.hxx>

namespace TopologicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;
	class CellComplex;

	class CellGUID
	{
	public:
		static std::string Get()
		{
			return std::string("8bda6c76-fa5c-4288-9830-80d32d283251");
		}
	};

	class Cell : public Topology
	{
	public:
		typedef std::shared_ptr<Cell> Ptr;

	public:
		/// <summary>
		/// <para></para>
		/// </summary>
		/// <param name="rkOcctSolid"></param>
		/// <param name="rkOcctLabel"></param>
		TOPOLOGIC_API Cell(const TopoDS_Solid& rkOcctSolid, const std::string& rkGuid = "");
		virtual ~Cell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AdjacentCells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCellComplexes"></param>
		/// <returns></returns>
		TOPOLOGIC_API void CellComplexes(std::list<std::shared_ptr<CellComplex>>& rCellComplexes) const;

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
		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkFaces"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cell> ByFaces(const std::list<std::shared_ptr<Face>>& rkFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkSHell"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Cell> ByShell(const std::shared_ptr<Shell>& rkShell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherCell"></param>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void SharedEdges(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherCell"></param>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void SharedFaces(const std::shared_ptr<Cell>& kpAnotherCell, std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherCell"></param>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void SharedVertices(const std::shared_ptr<Cell>& kpkAnotherCell, std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Shell> ExternalBoundary() const;


		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InternalBoundaries(std::list<std::shared_ptr<Shell>>& rShells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkParentTopology"></param>
		/// <returns></returns>
		virtual bool IsManifold() const;

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
		virtual TopoDS_Solid& GetOcctSolid();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Solid& GetOcctSolid() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctSolid"></param>
		void SetOcctSolid(const TopoDS_Solid& rkOcctSolid);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		virtual TopologyType GetType() const { return TOPOLOGY_CELL; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return CellGUID::Get();
		}

		static TOPOLOGIC_API int Type() { return TopologicCore::TOPOLOGY_CELL; }
		
		virtual bool IsContainerType() { return false; }

	protected:
		/// <summary>
		/// The underlying OCCT cell.
		/// </summary>
		TopoDS_Solid m_occtSolid;
	};
}
