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
		TOPOLOGIC_API void AdjacentFaces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void Apertures(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Cells(std::list<Cell*>& rCells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API bool IsApplied() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		TOPOLOGIC_API void Shells(std::list<Shell*>& rShells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		TOPOLOGIC_API void Vertices(std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rWires"></param>
		TOPOLOGIC_API void Wires(std::list<Wire*>& rWires) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkApertures"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool ApplyApertures(const std::list<Face*>& rkApertures) const;

		/// <summary>
		/// 
		/// </summary>
		TOPOLOGIC_API double Area() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <returns></returns>
		TOPOLOGIC_API static Face* ByClosedWire(Wire const * const kpkWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static Face* ByEdges(const std::list<Edge*>& rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="pOcctSurface"></param>
		/// <returns></returns>
		static Face* BySurface(Handle(Geom_Surface) pOcctSurface);

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
		/// <param name="kpkOuterWire"></param>
		/// <param name="rkInnerWires"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Face* BySurface(
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
			Wire const * const kpkOuterWire,
			const std::list<Wire*>& rkInnerWires);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherFace"></param>
		/// <param name="rEdges"></param>
		void TOPOLOGIC_API SharedEdges(Face const * const kpkAnotherFace, std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkAnotherFace"></param>
		/// <param name="rVertices"></param>
		void TOPOLOGIC_API SharedVertices(Face const * const kpkAnotherFace, std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		TOPOLOGIC_API Wire* OuterWire() const;

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
		/// The underlying OCCT face.
		/// </summary>
		TopoDS_Face* m_pOcctFace;
	};
}
