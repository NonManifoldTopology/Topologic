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

		TOPOLOGIC_API Graph(const Graph* rkAnotherGraph);

		virtual ~Graph();

		TOPOLOGIC_API Topology::Ptr Topology() const;

		TOPOLOGIC_API void Vertices(std::list<std::shared_ptr<Vertex>>& vertices) const;

		TOPOLOGIC_API void Edges(std::list<std::shared_ptr<Edge>>& edges) const;

		TOPOLOGIC_API void AddVertex(const std::shared_ptr<Vertex>& kpVertex, const bool kAddToVerticesList);

		TOPOLOGIC_API void AddEdge(const std::shared_ptr<Edge>& kpEdge);

		TOPOLOGIC_API int VertexDegree(const std::shared_ptr<Vertex>& kpVertex) const;

		TOPOLOGIC_API int VertexDegree(const TopoDS_Vertex& kpVertex) const;

		TOPOLOGIC_API void AdjacentVertices(const std::shared_ptr<Vertex>& kpVertex, std::list<std::shared_ptr<Vertex>>& rAdjacentVertices) const;

		TOPOLOGIC_API void Connect(const std::shared_ptr<Vertex>& kpVertex1, const std::shared_ptr<Vertex>& kpVertex2);

		TOPOLOGIC_API bool ContainsVertex(const std::shared_ptr<Vertex>& kpVertex) const;

		TOPOLOGIC_API bool ContainsEdge(const std::shared_ptr<Edge>& kpEdge);

		TOPOLOGIC_API void DegreeSequence(std::list<int>& rDegreeSequence) const;

		TOPOLOGIC_API double Density() const;

	protected:

		static Graph::Ptr ByVertex(
			const std::shared_ptr<Vertex> kpVertex,
			const bool kToExteriorApertures);

		static Graph::Ptr ByEdge(
			const std::shared_ptr<Edge> kpEdge,
			const bool kDirect,
			const bool kToExteriorApertures);

		static Graph::Ptr ByWire(const std::shared_ptr<Wire> kpWire,
			const bool kDirect,
			const bool kToExteriorApertures);

		static Graph::Ptr ByFace(const std::shared_ptr<Face> kpFace,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static Graph::Ptr ByShell(const std::shared_ptr<Shell> kpShell,
			const bool kDirect,
			const bool kViaSharedTopologies,
			const bool kViaSharedApertures,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static Graph::Ptr ByCell(const std::shared_ptr<Cell> kpCell,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static Graph::Ptr ByCellComplex(const std::shared_ptr<CellComplex> kpCellComplex,
			const bool kDirect,
			const bool kViaSharedTopologies,
			const bool kViaSharedApertures,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static Graph::Ptr ByCluster(const std::shared_ptr<Cluster> kpCluster,
			const bool kDirect,
			const bool kViaSharedTopologies,
			const bool kViaSharedApertures,
			const bool kToExteriorTopologies,
			const bool kToExteriorApertures);

		static bool AreVerticesGeometricallyIdentical(const TopoDS_Vertex& rkOcctVertex1, const TopoDS_Vertex& rkOcctVertex2, const double kDistanceThreshold);

		std::shared_ptr<Vertex> GetGeometricallyIdenticalVertexOrAddVertex(const std::shared_ptr<Vertex>& kpVertex, const bool kAddToDictionary, const bool kAddToVerticesList);

		TopoDS_Vertex GetGeometricallyIdenticalVertex(const TopoDS_Vertex& rkOcctVertex) const;

		std::shared_ptr<Vertex> GetGeometricallyIdenticalVertex(const std::shared_ptr<Vertex>& kpQueryVertex) const;

		std::list<Vertex::Ptr> m_vertices;
		std::list<Edge::Ptr> m_edges;

		std::map<TopoDS_Vertex, TopTools_MapOfShape, OcctShapeComparator> m_graphDictionary;
	};
}