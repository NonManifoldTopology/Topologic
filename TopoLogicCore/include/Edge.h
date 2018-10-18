#pragma once

#include "Topology.h"
#include "Utilities.h"

#include <list>

#include <Standard_Handle.hxx>
#include <Geom_Curve.hxx>
#include <TopoDS_Edge.hxx>
#include <TColgp_Array1OfPnt.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <BRepBuilderAPI_EdgeError.hxx>

namespace TopologicCore
{
	class Vertex;
	class Wire;

	class EdgeGUID
	{
	public:
		static std::string Get()
		{
			return std::string("1fc6e6e1-9a09-4c0a-985d-758138c49e35");
		}
	};

	class Edge : public Topology
	{
	public:
		typedef std::shared_ptr<Edge> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctEdge"></param>
		TOPOLOGIC_API Edge(const TopoDS_Edge& rkOcctEdge, const std::string& rkGuid = "");

		virtual ~Edge();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpHostTopology"></param>
		/// <param name="rEdges"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AdjacentEdges(const std::shared_ptr<Topology>& kpHostTopology, std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpHostTopology"></param>
		/// <param name="rWires"></param>
		/// <returns></returns>
		TOPOLOGIC_API void Wires(const std::shared_ptr<Topology>& kpHostTopology, std::list<std::shared_ptr<Wire>>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctPoles"></param>
		/// <param name="rkOcctWeights"></param>
		/// <param name="rkOcctKnots"></param>
		/// <param name="rkOcctMultiplicities"></param>
		/// <param name="kDegree"></param>
		/// <param name="kIsPeriodic"></param>
		/// <param name="kIsRational"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Edge::Ptr ByCurve(
			const TColgp_Array1OfPnt& rkOcctPoles, 
			const TColStd_Array1OfReal& rkOcctWeights, 
			const TColStd_Array1OfReal& rkOcctKnots, 
			const TColStd_Array1OfInteger& rkOcctMultiplicities, 
			const int kDegree, 
			const bool kIsPeriodic = false,
			const bool kIsRational = true);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCurve"></param>
		/// <param name="rkParameter1">The first parameter, ranging between 0 and 1.</param>
		/// <param name="rkParameter2">The second parameter, ranging between 0 and 1. Must be larger than rkParameter1, otherwise they will be swapped.</param>
		/// <returns></returns>
		static TOPOLOGIC_API Edge::Ptr ByCurve(Handle(Geom_Curve) pOcctCurve, const double rkParameter1 = 0.0, const double rkParameter2 = 1.0);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpStartVertex"></param>
		/// <param name="kpEndVertex"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Edge::Ptr ByStartVertexEndVertex(const std::shared_ptr<Vertex>& kpStartVertex, const std::shared_ptr<Vertex>& kpEndVertex);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherEdge"></param>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Vertex> SharedVertex(const Edge::Ptr& kpAnotherEdge) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkParentTopology"></param>
		/// <returns></returns>
		virtual bool IsManifold(TopologicCore::Topology const * const kpkParentTopology) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
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
		/// <returns></returns>
		virtual TopoDS_Edge& GetOcctEdge();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Edge& GetOcctEdge() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctShape"></param>
		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctEdge"></param>
		void SetOcctEdge(const TopoDS_Edge& rkOcctEdge);

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Handle(Geom_Curve) Curve() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rU0"></param>
		/// <param name="rU1"></param>
		/// <returns></returns>
		TOPOLOGIC_API Handle(Geom_Curve) Curve(double& rU0, double& rU1) const;

		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		virtual TopologyType GetType() const { return TOPOLOGY_EDGE; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return EdgeGUID::Get();
		}

		/// <summary>
		/// 
		/// </summary>
		/// <param name="occtEdgeError"></param>
		static TOPOLOGIC_API void Throw(const BRepBuilderAPI_EdgeError occtEdgeError);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kOcctMinParameter"></param>
		/// <param name="kOcctMaxParameter"></param>
		/// <param name="kNonNormalizedParameter"></param>
		/// <returns></returns>
		static TOPOLOGIC_API double NormalizeParameter(const double kOcctMinParameter, const double kOcctMaxParameter, const double kNonNormalizedParameter);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kOcctMinParameter"></param>
		/// <param name="kOcctMaxParameter"></param>
		/// <param name="kNormalizedParameter"></param>
		/// <returns></returns>
		static TOPOLOGIC_API double NonNormalizeParameter(const double kOcctMinParameter, const double kOcctMaxParameter, const double kNormalizedParameter);

	protected:
		/// <summary>
		/// The underlying OCCT edge.
		/// </summary>
		TopoDS_Edge m_occtEdge;
	};
}
