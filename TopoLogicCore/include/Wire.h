#pragma once

#include "Topology.h"
#include "Utilities.h"

#include <list>

#include <TopoDS_Wire.hxx>

class BRepBuilderAPI_MakeWire;

namespace TopologicCore
{
	class Vertex;
	class Edge;
	class Face;

	class WireGUID
	{
	public:
		static std::string Get()
		{
			return std::string("b99ccd99-6756-401d-ab6c-11162de541a3");
		}
	};

	class Wire : public Topology
	{
	public:
		typedef std::shared_ptr<Wire> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctWire"></param>
		TOPOLOGIC_API Wire(const TopoDS_Wire& rkOcctWire, const std::string& rkGuid = "");

		TOPOLOGIC_API virtual ~Wire();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		void TOPOLOGIC_API Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		/// <returns></returns>
		void TOPOLOGIC_API Faces(std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool TOPOLOGIC_API IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		void TOPOLOGIC_API Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Wire> ByEdges(const std::list<std::shared_ptr<Edge>>& rkEdges);

		static TopoDS_Wire ByOcctEdges(const TopTools_ListOfShape& rkOcctEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual bool IsManifold() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API int GetNumberOfBranches() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		TOPOLOGIC_API virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual TopoDS_Shape& GetOcctShape();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual const TopoDS_Shape& GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		TOPOLOGIC_API virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctWire"></param>
		TOPOLOGIC_API void SetOcctWire(const TopoDS_Wire& rkOcctWire);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual TopoDS_Wire& GetOcctWire();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API virtual const TopoDS_Wire& GetOcctWire() const;

		TOPOLOGIC_API virtual std::shared_ptr<Vertex> CenterOfMass() const;

		virtual TopologyType GetType() const { return TOPOLOGY_WIRE; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return WireGUID::Get();
		}

		static TOPOLOGIC_API int Type() { return TopologicCore::TOPOLOGY_WIRE; }

		virtual bool IsContainerType() { return true; }

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctMakeWire"></param>
		static TOPOLOGIC_API void Throw(const BRepBuilderAPI_MakeWire& rkOcctMakeWire);

	protected:
		/// <summary>
		/// The underlying OCCT wire.
		/// </summary>
		TopoDS_Wire m_occtWire;
	};
}
