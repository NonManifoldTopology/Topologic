#include <Wire.h>
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>
//#include <GlobalCluster.h>
#include <OcctCounterAttribute.h>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <ShapeFix_Wire.hxx>
#include <StdFail_NotDone.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	void Wire::Edges(std::list<std::shared_ptr<Edge>>& rEdges) const
	{
		// This query uses a specialised class BRepTools_WireExplorer 
		for (BRepTools_WireExplorer occtWireExplorer(GetOcctWire()); occtWireExplorer.More(); occtWireExplorer.Next())
		{
			const TopoDS_Edge& rkOcctEdge = occtWireExplorer.Current();
			TDF_Label occtEdgeLabel;
			LabelManager::GetInstance().FindChildLabelByShape(rkOcctEdge, occtEdgeLabel);
			rEdges.push_back(TopologicalQuery::Downcast<Edge>(Topology::ByOcctShape(rkOcctEdge, occtEdgeLabel)));
		}
	}

	void Wire::Faces(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Face>>& rFaces) const
	{
		UpwardNavigation(kpParentTopology, rFaces);
	}

	bool Wire::IsClosed() const
	{
		ShapeAnalysis_Wire shapeAnalysisWire;
		shapeAnalysisWire.Load(GetOcctWire());
		return shapeAnalysisWire.CheckClosed();
	}

	void Wire::Vertices(std::list<std::shared_ptr<Vertex>>& rVertices) const
	{
		TopoDS_Edge lastEdge;
		for(BRepTools_WireExplorer occtWireExplorer(GetOcctWire()); occtWireExplorer.More(); occtWireExplorer.Next())
		{
			const TopoDS_Vertex& rkOcctVertex = occtWireExplorer.CurrentVertex();
			lastEdge = occtWireExplorer.Current();
			TDF_Label occtVertexLabel;
			LabelManager::GetInstance().FindChildLabelByShape(rkOcctVertex, occtVertexLabel);
			rVertices.push_back(TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex, occtVertexLabel)));
		}

		// Add the last one.
		if (!lastEdge.IsNull())
		{
			TopoDS_Vertex occtLastVertex = TopExp::LastVertex(lastEdge);
			TDF_Label occtVertexLabel;
			LabelManager::GetInstance().FindChildLabelByShape(occtLastVertex, occtVertexLabel);
			rVertices.push_back(TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(occtLastVertex, occtVertexLabel)));
		}
	}

	// This method may involve making copies of the edges if they originally do not share vertices.
	std::shared_ptr<Wire> Wire::ByEdges(const std::list<std::shared_ptr<Edge>>& rkEdges)
	{
		if (rkEdges.empty())
		{
			throw std::exception("No edge is passed.");
		}

		TopTools_ListOfShape occtEdges;

		for(const std::shared_ptr<Edge>& kpEdge : rkEdges)
		{
			occtEdges.Append(kpEdge->GetOcctShape());
		}

		BRepBuilderAPI_MakeWire occtMakeWire;
		occtMakeWire.Add(occtEdges);

		try {
			std::shared_ptr<Wire> pWire = std::make_shared<Wire>(occtMakeWire);

			std::list<std::pair<std::shared_ptr<Topology>, std::shared_ptr<Topology>>> topologyPairs;
			std::list<std::shared_ptr<Topology>> members;
			pWire->Members(members);
			for (const std::shared_ptr<Topology>& kpMember : members)
			{
				topologyPairs.push_back(std::make_pair(kpMember, kpMember));
			}

			// Add the edges to the wire's label. Must do this manually because of the Modified()'s nature to map 
			// old to new sub-shapes.
			LabelManager::GetInstance().AddModifiedMembers(
				pWire->GetOcctLabel(),
				topologyPairs);

			return pWire;
		}
		catch (StdFail_NotDone&)
		{
			Throw(occtMakeWire);
			return nullptr;
		}
	}

	void Wire::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of curves
		std::list<std::shared_ptr<Edge>> edges;
		Edges(edges);

		for (const std::shared_ptr<Edge>& kpEdge : edges)
		{
			rOcctGeometries.push_back(kpEdge->Curve());
		}
	}

	TopoDS_Shape& Wire::GetOcctShape()
	{
		return GetOcctWire();
	}

	const TopoDS_Shape& Wire::GetOcctShape() const
	{
		return GetOcctWire();
	}

	TopoDS_Wire& Wire::GetOcctWire()
	{
		assert(!m_occtWire.IsNull() && "Wire::m_occtWire is null.");
		if (m_occtWire.IsNull())
		{
			throw std::exception("Wire::m_occtWire is null.");
		}

		return m_occtWire;
	}

	const TopoDS_Wire& Wire::GetOcctWire() const
	{
		assert(!m_occtWire.IsNull()  && "Wire::m_occtWire is null.");
		if (m_occtWire.IsNull())
		{
			throw std::exception("Wire::m_occtWire is null.");
		}

		return m_occtWire;
	}

	void Wire::Throw(const BRepBuilderAPI_MakeWire & rkOcctMakeWire)
	{
		// The error messages are based on those in the OCCT documentation.
		// https://www.opencascade.com/doc/occt-7.2.0/refman/html/_b_rep_builder_a_p_i___wire_error_8hxx.html

		switch (rkOcctMakeWire.Error())
		{
		case BRepBuilderAPI_EmptyWire:
			throw std::exception("No initialization of the algorithm. Only an empty constructor was used.");

		case BRepBuilderAPI_DisconnectedWire:
			throw std::exception("The last edge which you attempted to add was not connected to the wire.");

		case BRepBuilderAPI_NonManifoldWire:
			throw std::exception("The wire has some singularity.");

		default: // i.e. BRepBuilderAPI_WireDone 
			throw std::exception("A wire was successfully created.");
		}
	}

	Wire::Wire(const TopoDS_Wire& rkOcctWire, const TDF_Label& rkOcctLabel)
		: Topology(1)
		, m_occtWire(rkOcctWire)
	{
		//GlobalCluster::GetInstance().Add(this);
		SetOcctLabel(rkOcctLabel);
		OcctCounterAttribute::IncreaseCounter(GetOcctLabel());
	}

	Wire::~Wire()
	{
		//GlobalCluster::GetInstance().Remove(this);
		DecreaseCounter();
	}
}
