#pragma once

#include "Topology.h"

#include <list>

#include <TopoDS_Compound.hxx>
#include <TopoDS_Builder.hxx>

namespace TopoLogicCore
{
	class Cell;
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;

	class Cluster : public Topology
	{
	public:
		/// <summary>
		/// 
		/// </summary>
		/// <param name="kAddToGlobalCluster"></param>
		Cluster(const bool kAddToGlobalCluster = true);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpOcctCompound"></param>
		/// <param name="kAddToGlobalCluster"></param>
		Cluster(TopoDS_Compound * const kpOcctCompound, const bool kAddToGlobalCluster = true);

		virtual ~Cluster();

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rkTopologies"></param>
		/// <returns></returns>
		static TOPOLOGIC_API Cluster* ByTopology(const std::list<Topology*>& rkTopologies);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpCluster"></param>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool Add(Topology const * const kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		/// <param name="kpkTopology"></param>
		/// <returns></returns>
		TOPOLOGIC_API bool Remove(Topology const * const kpkTopology);

		/// <summary>
		/// 
		/// </summary>
		virtual TopoDS_Shape* GetOcctShape() const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rOcctGeometries"></param>
		virtual void Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const;




		/// <summary>
		/// 
		/// </summary>
		/// <param name="rShells"></param>
		TOPOLOGIC_API void Shells(std::list<Shell*>& rShells) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rEdges"></param>
		TOPOLOGIC_API void Edges(std::list<Edge*>& rEdges) const;

		/// <summary>
		/// 
		/// </summary>
		/// <param name="rFaces"></param>
		TOPOLOGIC_API void Faces(std::list<Face*>& rFaces) const;

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
		/// <param name="rCells"></param>
		TOPOLOGIC_API void Cells(std::list<Cell*>& rCells) const;

	protected:
		/// <summary>
		/// The underlying OCCT compound.
		/// </summary>
		TopoDS_Compound* m_pOcctCompound;

		TopoDS_Builder m_occtBuilder;
	};
}
