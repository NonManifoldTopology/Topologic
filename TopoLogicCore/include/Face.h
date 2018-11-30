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
		TOPOLOGIC_API Face(const TopoDS_Face& rkOcctFace, const std::string& rkGuid = "");

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
		/// <returns></returns>
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
		/// <returns></returns>
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
		static TOPOLOGIC_API std::shared_ptr<Face> ByEdges(const std::list<std::shared_ptr<Edge>>& rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctSurface"></param>
		/// <returns></returns>
		static TOPOLOGIC_API std::shared_ptr<Face> BySurface(Handle(Geom_Surface) pOcctSurface);

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
		TOPOLOGIC_API std::shared_ptr<Wire> ExternalBoundary() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rInternalBoundaries"></param>
		/// <returns></returns>
		TOPOLOGIC_API void InternalBoundaries(std::list<std::shared_ptr<Wire>>& rInternalBoundaries) const;

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
		/// <returns></returns>
		virtual bool IsManifold() const;

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
		/// <param name="rkOcctShape"></param>
		virtual void SetOcctShape(const TopoDS_Shape& rkOcctShape);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctFace"></param>
		void SetOcctFace(const TopoDS_Face& rkOcctFace);

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

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkOcctMakeFace"></param>
		static TOPOLOGIC_API void Throw(const BRepBuilderAPI_MakeFace& rkOcctMakeFace);

		static TOPOLOGIC_API int Type() { return TopologicCore::TOPOLOGY_FACE; }

		virtual bool IsContainerType() { return false; }

	protected:
		/// <summary>
		/// The underlying OCCT face.
		/// </summary>
		TopoDS_Face m_occtFace;
	};
}
