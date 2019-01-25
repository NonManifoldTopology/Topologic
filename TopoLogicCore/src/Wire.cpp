#include "Wire.h"
#include "Vertex.h"
#include "Edge.h"
#include "Face.h"
#include "WireFactory.h"
#include "GlobalCluster.h"

#include <BRepBuilderAPI_MakeWire.hxx>
#include <BRepCheck_Wire.hxx>
#include <BRepGProp.hxx>
#include <BRepTools_WireExplorer.hxx>
#include <Geom_CartesianPoint.hxx>
#include <GProp_GProps.hxx>
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
		BRepTools_WireExplorer occtWireExplorer(GetOcctWire());
		const TopoDS_Vertex& rkOcctFirstVertex = occtWireExplorer.CurrentVertex();
		Vertex::Ptr pFirstVertex = std::make_shared<Vertex>(rkOcctFirstVertex);
		std::list<Edge::Ptr> adjacentEdgesToFirstVertex;
		pFirstVertex->Edges(adjacentEdgesToFirstVertex);

		int numOfBranches = GetNumberOfBranches();
		if(numOfBranches >= 0 
		  ||
		  (!IsClosed() && adjacentEdgesToFirstVertex.size() > 1)) // open and the first vertex is adjacent to > 1 edges
		{
			DownwardNavigation(rEdges);
		}else
		{
			// This query uses a specialised class BRepTools_WireExplorer 
			for (BRepTools_WireExplorer occtWireExplorer(GetOcctWire()); occtWireExplorer.More(); occtWireExplorer.Next())
			{
				const TopoDS_Edge& rkOcctEdge = occtWireExplorer.Current();
				rEdges.push_back(std::make_shared<Edge>(rkOcctEdge, EdgeGUID::Get()));
			}
		}
	}

	void Wire::Faces(std::list<Face::Ptr>& rFaces) const
	{
		UpwardNavigation(rFaces);
	}

	bool Wire::IsClosed() const
	{
		BRepCheck_Wire occtCheckWire(TopoDS::Wire(GetOcctShape()));
		BRepCheck_Status status = occtCheckWire.Closed();
		bool isClosed = status == BRepCheck_NoError;
		return isClosed;
	}

	void Wire::Vertices(std::list<Vertex::Ptr>& rVertices) const
	{
		int numOfBranches = GetNumberOfBranches();
		if (numOfBranches == 0)
		{
			TopoDS_Edge lastEdge;
			for (BRepTools_WireExplorer occtWireExplorer(GetOcctWire()); occtWireExplorer.More(); occtWireExplorer.Next())
			{
				const TopoDS_Vertex& rkOcctVertex = occtWireExplorer.CurrentVertex();
				lastEdge = occtWireExplorer.Current();
				rVertices.push_back(std::make_shared<Vertex>(rkOcctVertex, VertexGUID::Get()));
			}

			// If the wire is open, add the last one. This is not needed for a closed wire.
			if (!IsClosed() && !lastEdge.IsNull())
			{
				TopoDS_Vertex occtLastVertex = TopExp::LastVertex(lastEdge);
				rVertices.push_back(std::make_shared<Vertex>(occtLastVertex));
			}
		}
		else
		{
			DownwardNavigation(rVertices);
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

		TopoDS_Wire occtWire = ByOcctEdges(occtEdges);
		Wire::Ptr pWire = std::make_shared<Wire>(occtWire);
		Wire::Ptr pCopyWire = std::dynamic_pointer_cast<Wire>(pWire->DeepCopy());

		GlobalCluster::GetInstance().AddTopology(pCopyWire->GetOcctWire());
		return pCopyWire;
	}

	TopoDS_Wire Wire::ByOcctEdges(const TopTools_ListOfShape & rkOcctEdges)
	{
		BRepBuilderAPI_MakeWire occtMakeWire;
		occtMakeWire.Add(rkOcctEdges);

		try {
			TransferMakeShapeContents(occtMakeWire, rkOcctEdges);

			return occtMakeWire;
		}
		catch (StdFail_NotDone&)
		{
			Throw(occtMakeWire);
			return TopoDS_Wire();
		}
	}

	bool Wire::IsManifold() const
	{
		return false;
	}

	int Wire::GetNumberOfBranches() const
	{
		std::list<Vertex::Ptr> vertices;
		DownwardNavigation<Vertex>(vertices);

		int numOfBranches = 0;
		for (const Vertex::Ptr& kpVertex: vertices)
		{
			std::list<Edge::Ptr> edges;
			kpVertex->UpwardNavigation<Edge>(GetOcctWire(), edges);
			if (edges.size() > 2)
			{
				numOfBranches++;
			}
		}

		return numOfBranches;
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

	void Wire::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		try {
			SetOcctWire(TopoDS::Wire(rkOcctShape));
		}
		catch (Standard_Failure e)
		{
			throw std::exception(e.GetMessageString());
		}
	}

	void Wire::SetOcctWire(const TopoDS_Wire & rkOcctWire)
	{
		m_occtWire = rkOcctWire;
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

	std::shared_ptr<Vertex> Wire::CenterOfMass() const
	{
		GProp_GProps occtShapeProperties;
		BRepGProp::LinearProperties(GetOcctShape(), occtShapeProperties);
		return Vertex::ByPoint(new Geom_CartesianPoint(occtShapeProperties.CentreOfMass()));
	}

	std::string Wire::GetTypeAsString() const
	{
		return std::string("Wire");
	}

	Wire::Wire(const TopoDS_Wire& rkOcctWire, const std::string& rkGuid)
		: Topology(1, rkOcctWire, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtWire(rkOcctWire)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<WireFactory>());
	}

	Wire::~Wire()
	{

	}
}
