#pragma once

#include "Topology.h"
#include "Utilities.h"

#include <Standard_Handle.hxx>
#include <Geom_Geometry.hxx>
#include <TopoDS_Vertex.hxx>

class Geom_Point;
class gp_Pnt;

namespace TopologicCore
{
	class Edge;
	
	class VertexGUID
	{
	public:
		static std::string Get()
		{
			return std::string("c4a9b420-edaf-4f8f-96eb-c87fbcc92f2b");
		}
	};

	/// <summary>
	/// The representation of a topological vertex. This class wraps OCCT's TopoDS_Vertex.
	/// </summary>
	class Vertex : public Topology
	{
	public:
		typedef std::shared_ptr<Vertex> Ptr;

	public:
		/// <summary>
		/// Creates a vertex by an OCCT vertex.
		/// </summary>
		/// <param name="rkOcctVertex">An OCCT vertex</param>
		Vertex(const TopoDS_Vertex& rkOcctVertex, const std::string& rkGuid = "");

		virtual ~Vertex();

		/// <summary>
		/// Creates a vertex by a point.
		/// </summary>
		/// <param name="pOcctPoint">An OCCT point</param>
		/// <returns>The created Topologic vertex.</returns>
		static TOPOLOGIC_API std::shared_ptr<Vertex> ByPoint(Handle(Geom_Point) pOcctPoint);

		/// <summary>
		/// Returns the list of edges of which this vertex is a constituent member.
		/// </summary>
		/// <param name="kpHostTopology"></param>
		/// <param name="rEdges">The edges containing this vertex as a constituent member</param>
		TOPOLOGIC_API void Edges(const std::shared_ptr<Topology>& kpHostTopology, std::list<std::shared_ptr<Edge>>& rEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API double X() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API double Y() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API double Z() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::tuple<double, double, double> Coordinate() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkParentTopology"></param>
		/// <returns></returns>
		virtual bool IsManifold(TopologicCore::Topology const * const kpkParentTopology) const;

		/// <summary>
		/// Return the corresponding point of this vertex. The output list only contains one vertex.
		/// </summary>
		/// <exception cref="std::exception">Null OCCT vertex</exception>
		/// <param name="rOcctGeometries">The output parameter, containing only one point corresponding to this vertex.</param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

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
		/// <param name="rkOcctShape"></param>
		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual TopoDS_Vertex& GetOcctVertex();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctVertex"></param>
		void SetOcctVertex(const TopoDS_Vertex& rkOcctVertex);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Vertex& GetOcctVertex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Handle(Geom_Point) Point() const;

		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		virtual TopologyType GetType() const { return TOPOLOGY_VERTEX; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return VertexGUID::Get();
		}

	protected:
		/// <summary>
		/// The underlying OCCT vertex.
		/// </summary>
		TopoDS_Vertex m_occtVertex;
	};
}