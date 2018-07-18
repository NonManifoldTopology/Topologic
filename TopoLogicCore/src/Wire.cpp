#include <Wire.h>
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>
//#include <GlobalCluster.h>
//#include <OcctCounterAttribute.h>

#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <ShapeAnalysis_Wire.hxx>
#include <ShapeFix_Wire.hxx>
#include <StdFail_NotDone.hxx>
#include <TopoDS.hxx>
#include <TopExp.hxx>
#include <TopExp_Explorer.hxx>

#include <assert.h>

namespace TopologicCore
{
	void Wire::Edges(std::list<Edge::Ptr>& rEdges) const
	{
		// This query uses a specialised class BRepTools_WireExplorer 
		for (BRepTools_WireExplorer occtWireExplorer(GetOcctWire()); occtWireExplorer.More(); occtWireExplorer.Next())
		{
			const TopoDS_Edge& rkOcctEdge = occtWireExplorer.Current();
			rEdges.push_back(TopologicalQuery::Downcast<Edge>(Topology::ByOcctShape(rkOcctEdge)));
		}

		// If still empty, use the DownwardNavigation
		if (rEdges.empty())
		{
			DownwardNavigation(rEdges);
		}
	}

	void Wire::Faces(const Topology::Ptr& kpParentTopology, std::list<Face::Ptr>& rFaces) const
	{
		UpwardNavigation(kpParentTopology, rFaces);
	}

	bool Wire::IsClosed() const
	{
		ShapeAnalysis_Wire shapeAnalysisWire;
		shapeAnalysisWire.Load(GetOcctWire());
		return shapeAnalysisWire.CheckClosed();
	}

	void Wire::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		TopoDS_Edge lastEdge;
		for(BRepTools_WireExplorer occtWireExplorer(GetOcctWire()); occtWireExplorer.More(); occtWireExplorer.Next())
		{
			const TopoDS_Vertex& rkOcctVertex = occtWireExplorer.CurrentVertex();
			lastEdge = occtWireExplorer.Current();
			rVertices.push_back(TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(rkOcctVertex)));
		}

		// Add the last one.
		if (!lastEdge.IsNull())
		{
			TopoDS_Vertex occtLastVertex = TopExp::LastVertex(lastEdge);
			rVertices.push_back(TopologicalQuery::Downcast<Vertex>(Topology::ByOcctShape(occtLastVertex)));
		}
	}

	// This method may involve making copies of the edges if they originally do not share vertices.
	Wire::Ptr Wire::ByEdges(const std::list<Edge::Ptr>& rkEdges)
	{
		if (rkEdges.empty())
		{
			throw std::exception("No edge is passed.");
		}

		TopTools_ListOfShape occtEdges;

		for(const Edge::Ptr& kpEdge : rkEdges)
		{
			occtEdges.Append(kpEdge->GetOcctShape());
		}

		BRepBuilderAPI_MakeWire occtMakeWire;
		occtMakeWire.Add(occtEdges);

		try {
			Wire::Ptr pWire = std::make_shared<Wire>(occtMakeWire);

			// Add the edges to the wire's label. Must do this manually because of the Modified()'s nature to map 
			// old to new sub-shapes.
			/*std::list<std::pair<Topology::Ptr, Topology::Ptr>> topologyPairs;
			std::list<Topology::Ptr> members;
			pWire->Members(members);
			for (const Topology::Ptr& kpMember : members)
			{
				topologyPairs.push_back(std::make_pair(kpMember, kpMember));
			}

			LabelManager::GetInstance().AddModifiedMembers(
				pWire->GetOcctLabel(),
				topologyPairs);*/

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
		std::list<Edge::Ptr> edges;
		Edges(edges);

		for (const Edge::Ptr& kpEdge : edges)
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

	Wire::Wire(const TopoDS_Wire& rkOcctWire)
		: Topology(1)
		, m_occtWire(rkOcctWire)
	{

	}

	Wire::~Wire()
	{

	}
}
