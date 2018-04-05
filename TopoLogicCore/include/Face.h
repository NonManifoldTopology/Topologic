#pragma once

#include "Topology.h"

#include <list>

#include <Standard_Handle.hxx>
#include <TopoDS_Face.hxx>
#include <TColgp_Array2OfPnt.hxx>
#include <TColStd_Array1OfInteger.hxx>
#include <TColStd_Array1OfReal.hxx>
#include <TColStd_Array2OfReal.hxx>

class BRepBuilderAPI_MakeFace;
class Geom_Surface;

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Shell;
	class Cell;

	/// <summary>
	/// 
	/// </summary>
	class Face : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctFace"></param>
		Face(const TopoDS_Face& rkOcctFace);

		virtual ~Face();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void AdjacentFaces(std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Cells(std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		TOPOLOGIC_API void Shells(std::list<std::shared_ptr<Shell>>& rShells) const;

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
		TOPOLOGIC_API double Area() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWire"></param>
		/// <returns></returns>
		TOPOLOGIC_API static std::shared_ptr<Face> ByClosedWire(const std::shared_ptr<Wire>& pkWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static std::shared_ptr<Face> ByEdges(const std::list<std::shared_ptr<Edge>>& rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctSurface"></param>
		/// <returns></returns>
		static std::shared_ptr<Face> BySurface(Handle(Geom_Surface) pOcctSurface);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctPoles"></param>
		/// <param name="rkOcctWeights"></param>
		/// <param name="rkOcctUKnots"></param>
		/// <param name="rkOcctVKnots"></param>
		/// <param name="rkOcctMultiplicities"></param>
		/// <param name="rkOcctVMultiplicities"></param>
		/// <param name="kUDegree"></param>
		/// <param name="kVDegree"></param>
		/// <param name="kIsUPeriodic"></param>
		/// <param name="kIsVPeriodic"></param>
		/// <param name="kIsRational"></param>
		/// <param name="rkOuterWire"></param>
		/// <param name="rkInnerWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Face> BySurface(
			const TColgp_Array2OfPnt& rkOcctPoles,
			const TColStd_Array2OfReal& rkOcctWeights,
			const TColStd_Array1OfReal& rkOcctUKnots,
			const TColStd_Array1OfReal& rkOcctVKnots,
			const TColStd_Array1OfInteger& rkOcctUMultiplicities,
			const TColStd_Array1OfInteger& rkOcctVMultiplicities,
			const int kUDegree,
			const int kVDegree,
			const bool kIsUPeriodic,
			const bool kIsVPeriodic,
			const bool kIsRational,
			const std::shared_ptr<Wire>& kpOuterWire,
			const std::list<std::shared_ptr<Wire>>& rkInnerWires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherFace"></param>
		/// <param name="rEdges"></param>
		void TOPOLOGIC_API SharedEdges(const std::shared_ptr<Face>& kpAnotherFace, std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpAnotherFace"></param>
		/// <param name="rVertices"></param>
		void TOPOLOGIC_API SharedVertices(const std::shared_ptr<Face>& kpAnotherFace, std::list<std::shared_ptr<Vertex>>& rVertices) const;


		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Wire> OuterBoundary() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rInnerBoundaries"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InnerBoundaries(std::list<std::shared_ptr<Wire>>& rInnerBoundaries) const;

		/// <summary>
		/// Note: TopoLogic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpVertex"></param>
		/// <param name="rU"></param>
		/// <param name="rV"></param>
		/// <returns></returns>
		TOPOLOGIC_API void UVParameterAtPoint(const std::shared_ptr<Vertex>& kpVertex, double& rU, double &rV) const;

		/// <summary>
		/// Note: TopoLogic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kU"></param>
		/// <param name="kV"></param>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Vertex> PointAtParameter(const double kU, const double kV) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpWire"></param>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Face> Trim(const std::shared_ptr<Wire>& kpWire) const;

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
		TOPOLOGIC_API Handle(Geom_Surface) Surface() const;

		virtual TopologyType GetType() const { return TOPOLOGY_FACE; }

	protected:
		/// <summary>
		/// Returns error messages when creating a face using OCCT's BRepBuilderAPI_MakeFace.
		/// </summary>
		/// <param name="rkOcctMakeFace">An instantiation of OCCT's BRepBuilderAPI_MakeFace</param>
		/// <returns>The error messsages</returns>
		static std::string GetOcctMakeFaceErrorMessage(const BRepBuilderAPI_MakeFace& rkOcctMakeFace);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctSurface"></param>
		/// <param name="kNonNormalizedU"></param>
		/// <param name="kNonNormalizedV"></param>
		/// <param name="rNormalizedU"></param>
		/// <param name="rNormalizedV"></param>
		static void NormalizeUV(Handle(Geom_Surface) pOcctSurface, const double kNonNormalizedU, const double kNonNormalizedV, double& rNormalizedU, double& rNormalizedV);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctSurface"></param>
		/// <param name="kNormalizedU"></param>
		/// <param name="kNormalizedV"></param>
		/// <param name="rNonNormalizedU"></param>
		/// <param name="rNonNormalizedV"></param>
		static void NonNormalizeUV(Handle(Geom_Surface) pOcctSurface, const double kNormalizedU, const double kNormalizedV, double& rNonNormalizedU, double& rNonNormalizedV);

		/// <summary>
		/// The underlying OCCT face.
		/// </summary>
		std::shared_ptr<TopoDS_Face> m_pOcctFace;
	};
}
