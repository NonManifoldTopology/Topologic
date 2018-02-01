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
		/// <param name="kpOcctEdge"></param>
		Edge(TopoDS_Edge * const kpOcctEdge);

		virtual ~Edge();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Vertex* StartVertex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Vertex* EndVertex() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<Wire*>& rWires) const;

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
		static TOPOLOGIC_API Edge* ByCurve(
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
		static TOPOLOGIC_API Edge* ByCurve(Handle(Geom_Curve) pOcctCurve, const double rkParameter1 = 0.0, const double rkParameter2 = 1.0);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkVertices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Edge* ByVertices(const std::list<Vertex*>& rkVertices);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherEdge"></param>
		/// <returns></returns>
		TOPOLOGIC_API Vertex* SharedVertex(Edge const * const kpkAnotherEdge) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		Handle(Geom_Curve) Curve() const;

	protected:
		/// <summary>
		/// The underlying OCCT edge.
		/// </summary>
		TopoDS_Edge* m_pOcctEdge;
	};
}
