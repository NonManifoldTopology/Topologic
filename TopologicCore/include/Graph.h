#pragma once

#include "Utilities.h"
#include "Vertex.h"
#include "Edge.h"

#include <list>

namespace TopologicCore
{
	class Vertex;
	class Edge;
	class Wire;
	class Face;
	class Shell;
	class Cell;
	class CellComplex;
	class Cluster;

	class Graph
	{
	public:
		typedef std::shared_ptr<Graph> Ptr;

	public:
		static TOPOLOGIC_API Graph::Ptr ByVerticesEdges(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges);

		static TOPOLOGIC_API Graph::Ptr ByTopology(
			const Topology::Ptr topology, 
			const bool kDirect,
			const bool kViaSharedTopologies, 
			const bool kViaSharedApertures, 
			const bool kToExteriorTopologies, 
			const bool kToExteriorApertures);

		Graph(const std::list<Vertex::Ptr>& rkVertices, const std::list<Edge::Ptr>& rkEdges);

		virtual ~Graph();

		TOPOLOGIC_API Topology::Ptr Topology();

	protected:

		static TOPOLOGIC_API Graph::Ptr ByVertex(
			const std::shared_ptr<Vertex> kpVertex,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByEdge(
			const std::shared_ptr<Edge> kpEdge,
			const bool kDirect,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByWire(const std::shared_ptr<Wire> kpWire,
			const bool kDirect,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByFace(const std::shared_ptr<Face> kpFace,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByShell(const std::shared_ptr<Shell> kpShell,
			const bool kDirect,
			const bool kViaSharedTopologies,
			const bool kViaSharedApertures,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByCell(const std::shared_ptr<Cell> kpCell,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByCellComplex(const std::shared_ptr<CellComplex> kpCellComplex,
			const bool kDirect,
			const bool kViaSharedTopologies,
			const bool kViaSharedApertures,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static TOPOLOGIC_API Graph::Ptr ByCluster(const std::shared_ptr<Cluster> kpCluster,
			const bool kDirect,
			const bool kViaSharedTopologies,
			const bool kViaSharedApertures,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		std::list<Vertex::Ptr> m_vertices;
		std::list<Edge::Ptr> m_edges;

		std::map<TopoDS_Vertex, TopTools_MapOfShape, OcctShapeComparator> m_graphDictionary;
	};
}