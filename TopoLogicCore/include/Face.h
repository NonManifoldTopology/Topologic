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

namespace TopologicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Shell;
	class Cell;

	class FaceGUID
	{
	public:
		static std::string Get()
		{
			return std::string("3b0a6afe-af86-4d96-a30d-d235e9c98475");
		}
	};

	/// <summary>
	/// 
	/// </summary>
	class Face : public Topology
	{
	public:
		typedef std::shared_ptr<Face> Ptr;

	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctFace"></param>
		Face(const TopoDS_Face& rkOcctFace, const std::string& rkGuid = "");

		virtual ~Face();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpParentTopology"></param>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void AdjacentFaces(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Face>>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpParentTopology"></param>
		/// <param name="rCells"></param>
		/// <returns></returns>
		TOPOLOGIC_API void Cells(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Cell>>& rCells) const;

		void Cells(Topology const * kpkParentTopology, std::list<std::shared_ptr<Cell>>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpParentTopology"></param>
		/// <param name="rShells"></param>
		/// <returns></returns>
		TOPOLOGIC_API void Shells(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Shell>>& rShells) const;

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
		virtual std::shared_ptr<Vertex> CenterOfMass() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpExternalBoundary"></param>
		/// <returns></returns>
		TOPOLOGIC_API static std::shared_ptr<Face> ByExternalBoundary(const std::shared_ptr<Wire>& kpExternalBoundary);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pkExternalBoundary"></param>
		/// <param name="rkInternalBoundaries"></param>
		/// <returns></returns>
		TOPOLOGIC_API static std::shared_ptr<Face> ByExternalInternalBoundaries(
			const std::shared_ptr<Wire>& pkExternalBoundary, 
			const std::list<std::shared_ptr<Wire>>& rkInternalBoundaries);

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

		/*/// <summary>
		/// 
		/// </summary>
		/// <param name="rkPoles"></param>
		/// <param name="rkWeights"></param>
		/// <param name="rkUKnots"></param>
		/// <param name="rkVKnots"></param>
		/// <param name="rkUMultiplicities"></param>
		/// <param name="rkVMultiplicities"></param>
		/// <param name="kUDegree"></param>
		/// <param name="kVDegree"></param>
		/// <param name="kIsUPeriodic"></param>
		/// <param name="kIsVPeriodic"></param>
		/// <param name="kIsRational"></param>
		/// <param name="kpOuterWire"></param>
		/// <param name="rkInnerWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Face> BySurface(
			const std::list<std::list<std::shared_ptr<Vertex>>>& rkPoles,
			const std::list<std::list<double>>& rkWeights,
			const std::list<double>& rkUKnots,
			const std::list<double>& rkVKnots,
			const std::list<int>& rkUMultiplicities,
			const std::list<int>& rkVMultiplicities,
			const int kUDegree,
			const int kVDegree,
			const bool kIsUPeriodic,
			const bool kIsVPeriodic,
			const bool kIsRational,
			const std::shared_ptr<Wire>& kpOuterWire,
			const std::list<std::shared_ptr<Wire>>& rkInnerWires);*/

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
		/// <param name="coreVertices"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Face::Ptr ByVertices(const std::list<std::list<std::shared_ptr<Vertex>>>& rkVertices);

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
		/// 
		/// </summary>
		/// <param name="kpWire"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AddInternalBoundary(const std::shared_ptr<Wire>& kpWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkWires"></param>
		/// <returns></returns>
		TOPOLOGIC_API void AddInternalBoundaries(const std::list<std::shared_ptr<Wire>>& rkWires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkParentTopology"></param>
		/// <returns></returns>
		virtual bool IsManifold(TopologicCore::Topology const * const kpkParentTopology) const;

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kpVertex"></param>
		/// <param name="rU"></param>
		/// <param name="rV"></param>
		/// <returns></returns>
		TOPOLOGIC_API void UVParameterAtPoint(const std::shared_ptr<Vertex>& kpVertex, double& rU, double &rV) const;

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kU"></param>
		/// <param name="kV"></param>
		/// <returns></returns>
		TOPOLOGIC_API std::shared_ptr<Vertex> PointAtParameter(const double kU, const double kV) const;

		/// <summary>
		/// Note: Topologic UV values are normalized (from 0 and 1), but OCCT's values are not normalized.
		/// </summary>
		/// <param name="kU"></param>
		/// <param name="kV"></param>
		/// <returns></returns>
		TOPOLOGIC_API gp_Dir NormalAtParameter(const double kU, const double kV) const;

		void UVSamplePoints(
			const std::list<double>& rkUValues,
			const std::list<double>& rkVValues,
			std::list<std::list<gp_Pnt>>& rSamplePoints,
			int& rNumUPoints,
			int& rNumVPoints,
			int& rNumUPanels,
			int& rNumVPanels,
			bool& rIsUClosed,
			bool& rIsVClosed);
			/*,
			std::list<double>& rOcctUValues,
			std::list<double>& rOcctVValues);*/

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
		virtual TopoDS_Face& GetOcctFace();

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual const TopoDS_Face& GetOcctFace() const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Handle(Geom_Surface) Surface() const;

		virtual TopologyType GetType() const { return TOPOLOGY_FACE; }

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		virtual std::string GetTypeAsString() const;

		virtual std::string GetClassGUID() const {
			return FaceGUID::Get();
		}

	protected:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctMakeFace"></param>
		static void Throw(const BRepBuilderAPI_MakeFace& rkOcctMakeFace);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kNonNormalizedU"></param>
		/// <param name="kNonNormalizedV"></param>
		/// <param name="rNormalizedU"></param>
		/// <param name="rNormalizedV"></param>
		void NormalizeUV(const double kNonNormalizedU, const double kNonNormalizedV, double& rNormalizedU, double& rNormalizedV) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kNormalizedU"></param>
		/// <param name="kNormalizedV"></param>
		/// <param name="rNonNormalizedU"></param>
		/// <param name="rNonNormalizedV"></param>
		void NonNormalizeUV(const double kNormalizedU, const double kNormalizedV, double& rNonNormalizedU, double& rNonNormalizedV) const;

		/// <summary>
		/// The underlying OCCT face.
		/// </summary>
		TopoDS_Face m_occtFace;
	};
}
