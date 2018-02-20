#pragma once

#include "Topology.h"
#include "Utilities.h"

#include <list>

#include <TopoDS_Wire.hxx>

class BRepBuilderAPI_MakeWire;

namespace TopoLogicCore
{
	class Vertex;
	class Edge;
	class Face;

	class Wire : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctWire"></param>
		Wire(TopoDS_Wire * const kpOcctWire);

		virtual ~Wire();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		void TOPOLOGIC_API Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		void TOPOLOGIC_API Faces(std::list<Face*>& rFaces) const;

		/// <summary>
		/// 
		/// </summary>
		/// <returns></returns>
		bool TOPOLOGIC_API IsClosed() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rVertices"></param>
		void TOPOLOGIC_API Vertices(std::list<Vertex*>& rVertices) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkEdges"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Wire* ByEdges(const std::list<Edge*>& rkEdges);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkWire"></param>
		/// <returns></returns>
		//static Wire* ByPolyCurve(TopoDS_Wire const * const kpkWire);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;

	protected:
		/// <summary>
		/// Returns error messages when creating an edge using OCCT's BRepBuilderAPI_MakeWire.
		/// </summary>
		/// <param name="rkOcctMakeWire">An instantiation of OCCT's BRepBuilderAPI_MakeWire</param>
		/// <returns>The error messsages</returns>
		static std::string GetOcctMakeWireErrorMessage(const BRepBuilderAPI_MakeWire& rkOcctMakeWire);

		/// <summary>
		/// The underlying OCCT wire.
		/// </summary>
		TopoDS_Wire* m_pOcctWire;
	};
}
