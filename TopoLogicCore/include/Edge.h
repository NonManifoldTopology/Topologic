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

namespace TopoLogicCore
{
	class Vertex;
	class Wire;

	class Edge : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctEdge"></param>
		Edge(const TopoDS_Edge& rkOcctEdge);

		virtual ~Edge();

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
		/// <param name="rkOcctPoles"></param>
		/// <param name="rkOcctWeights"></param>
		/// <param name="rkOcctKnots"></param>
		/// <param name="rkOcctMultiplicities"></param>
		/// <param name="kDegree"></param>
		/// <param name="kIsPeriodic"></param>
		/// <param name="kIsRational"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Edge> ByCurve(
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
		static TOPOLOGIC_API std::shared_ptr<Edge> ByCurve(Handle(Geom_Curve) pOcctCurve, const double rkParameter1 = 0.0, const double rkParameter2 = 1.0);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Edge> ByVertices(const std::list<std::shared_ptr<Vertex>>& rkVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherEdge"></param>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Vertex> SharedVertex(const std::shared_ptr<Edge>& kpAnotherEdge) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpVertex"></param>
		/// <returns></returns>
		TOPOLOGIC_API double ParameterAtPoint(const std::shared_ptr<Vertex>& kpVertex) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kParameter"></param>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Vertex> PointAtParameter(const double kParameter) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		virtual std::shared_ptr<TopoDS_Shape> GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Handle(Geom_Curve) Curve() const;

		virtual TopologyType GetType() const { return TOPOLOGY_EDGE; }

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCurve"></param>
		/// <param name="kNonNormalizedParameter"></param>
		/// <returns></returns>
		static double NormalizeParameter(Handle(Geom_Curve) pOcctCurve, const double kNonNormalizedParameter);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctCurve"></param>
		/// <param name="kNormalizedParameter"></param>
		/// <returns></returns>
		static double NonNormalizeParameter(Handle(Geom_Curve) pOcctCurve, const double kNormalizedParameter);


		/// <summary>
		/// The underlying OCCT edge.
		/// </summary>
		std::shared_ptr<TopoDS_Edge> m_pOcctEdge;
	};
}
