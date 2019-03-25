#pragma once

#include "Utilities.h"
#include "Vertex.h"
#include "Edge.h"

#include <list>
#include <chrono>

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

		TOPOLOGIC_API void AddVertices(const std::list<std::shared_ptr<Vertex>>& rkVertices);

		TOPOLOGIC_API void AddEdges(const std::list<std::shared_ptr<Edge>>& rkEdges);

		TOPOLOGIC_API int VertexDegree(const std::shared_ptr<Vertex>& kpVertex) const;

		TOPOLOGIC_API int VertexDegree(const TopoDS_Vertex& kpVertex) const;

		TOPOLOGIC_API void AdjacentVertices(const std::shared_ptr<Vertex>& kpVertex, std::list<std::shared_ptr<Vertex>>& rAdjacentVertices) const;

		TOPOLOGIC_API void Connect(const std::shared_ptr<Vertex>& kpVertex1, const std::shared_ptr<Vertex>& kpVertex2);

		TOPOLOGIC_API bool ContainsVertex(const std::shared_ptr<Vertex>& kpVertex) const;

		bool ContainsVertex(const TopoDS_Vertex& rkOcctVertex) const;

		TOPOLOGIC_API bool ContainsEdge(const std::shared_ptr<Edge>& kpEdge);

		TOPOLOGIC_API void DegreeSequence(std::list<int>& rDegreeSequence) const;

		TOPOLOGIC_API double Density() const;

		TOPOLOGIC_API bool IsComplete() const;

		TOPOLOGIC_API void IsolatedVertices(std::list<Vertex::Ptr>& rIsolatedVertices) const;

		TOPOLOGIC_API int MinimumDelta() const;

		TOPOLOGIC_API int MaximumDelta() const;

		TOPOLOGIC_API void AllPaths(
			const Vertex::Ptr& kpStartVertex,
			const Vertex::Ptr& kpEndVertex,
			const bool kUseTimeLimit,
			const double kTimeLimitInSeconds,
			std::list<std::shared_ptr<Wire>>& rPaths) const;

		void AllPaths(
			const Vertex::Ptr& kpStartVertex, 
			const Vertex::Ptr& kpEndVertex,
			const bool kUseTimeLimit,
			const double kTimeLimitInSeconds,
			const std::chrono::system_clock::time_point& rkStartingTime,
			std::list<Vertex::Ptr>& rPath,
			std::list<std::shared_ptr<Wire>>& rPaths) const;

		TOPOLOGIC_API std::shared_ptr<Wire> Path(
			const Vertex::Ptr& kpStartVertex,
			const Vertex::Ptr& kpEndVertex) const;

		std::shared_ptr<Wire> Path(
			const Vertex::Ptr& kpStartVertex,
			const Vertex::Ptr& kpEndVertex,
			std::list<Vertex::Ptr>& rPath) const;

		TOPOLOGIC_API std::shared_ptr<Wire> ShortestPath(
			const Vertex::Ptr& kpStartVertex,
			const Vertex::Ptr& kpEndVertex) const;

		std::shared_ptr<Wire> ShortestPath(
			const TopoDS_Vertex& rkOcctStartVertex,
			const TopoDS_Vertex& rkOcctEndVertex) const;

		TOPOLOGIC_API int Diameter() const;

		TOPOLOGIC_API int Distance(const std::shared_ptr<Vertex>& kpStartVertex, const std::shared_ptr<Vertex>& kpEndVertex) const;

		int Distance(const TopoDS_Vertex& rkOcctStartVertex, const TopoDS_Vertex& rkOcctVertex) const;

		TOPOLOGIC_API int Eccentricity(const std::shared_ptr<Vertex>& kpVertex) const;

		TOPOLOGIC_API bool IsErdoesGallai(const std::list<int>& rkSequence) const;

		TOPOLOGIC_API void RemoveVertices(const std::list<Vertex::Ptr>& rkVertices);

		TOPOLOGIC_API void RemoveEdges(const std::list<Edge::Ptr>& rkEdges);

	protected:

		typedef std::map<TopoDS_Vertex, TopTools_MapOfShape, OcctShapeComparator> GraphMap;

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

		static std::shared_ptr<Wire> ConstructPath(const std::list<Vertex::Ptr>& rkPathVertices);

		bool IsDegreeSequence(const std::list<int>& rkSequence) const;

		GraphMap m_graphDictionary;
	};
}