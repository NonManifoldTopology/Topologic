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

	class ShellGUID
	{
	public:
		static std::string Get()
		{
			return std::string("51c1e590-cec9-4e84-8f6b-e4f8c34fd3b3");
		}
	};

	/// <summary>
	/// 
	/// </summary>
	class Shell : public Topology
	{
	public:
		typedef std::shared_ptr<Shell> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShell"></param>
		TOPOLOGIC_API Shell(const TopoDS_Shell& rkOcctShell, const std::string& rkGuid = "");

		virtual ~Shell();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpParentTopology"></param>
		/// <param name="rCells"></param>
		/// <returns></returns>
		TOPOLOGIC_API void Cells(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Cell>>& rCells) const;

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

		static TopoDS_Shell ByOcctFaces(const TopTools_ListOfShape& rkOcctFaces);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkParentTopology"></param>
		/// <returns></returns>
		virtual bool IsManifold(TopologicCore::Topology const * const kpkParentTopology) const;

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
		/// <param name="rkOcctShape"></param>
		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShell"></param>
		void SetOcctShell(const TopoDS_Shell& rkOcctShell);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		virtual TopologyType GetType() const { return TOPOLOGY_SHELL; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return ShellGUID::Get();
		}

	protected:
		/// <summary>
		/// The underlying OCCT shell.
		/// </summary>
		TopoDS_Shell m_occtShell;
	};
}
