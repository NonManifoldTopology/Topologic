#include <Wire.h>
#include <Vertex.h>
#include <Edge.h>
#include <Face.h>
#include <GlobalCluster.h>

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
	void Wire::Edges(std::list<Edge*>& rEdges) const
	{
		int initialEdgeSize = (int)rEdges.size();
		BRepTools_WireExplorer occtWireExplorer;
		for (occtWireExplorer.Init(TopoDS::Wire(*GetOcctShape())); occtWireExplorer.More(); occtWireExplorer.Next())
		{
			rEdges.push_back(new Edge(occtWireExplorer.Current()));
		}

		if ((int)rEdges.size() > initialEdgeSize)
			return;

		// PATCH: 24/10/2017
		// If pEdges is still empty, e.g. due to defects (what defects?), try again with TopExp_Explorer

		TopTools_MapOfShape occtEdges;
		TopExp_Explorer occtExplorer;
		for (occtExplorer.Init(*GetOcctShape(), TopAbs_EDGE); occtExplorer.More(); occtExplorer.Next())
		{
			const TopoDS_Shape& occtCurrent = occtExplorer.Current();
			if (!occtEdges.Contains(occtCurrent))
			{
				occtEdges.Add(occtCurrent);
			}
		}

		for (TopTools_MapOfShape::const_iterator kOcctShapeIterator = occtEdges.cbegin();
			kOcctShapeIterator != occtEdges.cend();
			kOcctShapeIterator++)
		{
			rEdges.push_back(new Edge(TopoDS::Edge(*kOcctShapeIterator)));
		}
	}

	void Wire::Faces(std::list<Face*>& rFaces) const
	{
		TopTools_IndexedDataMapOfShapeListOfShape occtWireToFacesMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_WIRE, TopAbs_FACE, occtWireToFacesMap);

		const TopTools_ListOfShape& rkOcctFaces = occtWireToFacesMap.FindFromKey(*GetOcctShape());

		for (TopTools_ListOfShape::const_iterator kIterator = rkOcctFaces.cbegin();
			kIterator != rkOcctFaces.cend();
			kIterator++)
		{
			if (kIterator->ShapeType() == TopAbs_FACE)
			{
				rFaces.push_back(new Face(TopoDS::Face(*kIterator)));
			}
		}
	}

	bool Wire::IsClosed() const
	{
		ShapeAnalysis_Wire shapeAnalysisWire;
		shapeAnalysisWire.Load(TopoDS::Wire(*GetOcctShape()));
		return shapeAnalysisWire.CheckClosed();
	}

	void Wire::Vertices(std::list<Vertex*>& rVertices) const
	{
		BRepTools_WireExplorer occtWireExplorer;
		for (occtWireExplorer.Init(TopoDS::Wire(*GetOcctShape())); occtWireExplorer.More(); occtWireExplorer.Next())
		{
			rVertices.push_back(new Vertex(occtWireExplorer.CurrentVertex()));
		}
	}

	Wire* Wire::ByEdges(const std::list<Edge*>& rkEdges)
	{
		TopTools_ListOfShape occtEdges;
		for(std::list<Edge*>::const_iterator kEdgeIterator = rkEdges.begin();
			kEdgeIterator != rkEdges.end();
			kEdgeIterator++)
		{
			occtEdges.Append(*(*kEdgeIterator)->GetOcctShape());
		}

		BRepBuilderAPI_MakeWire occtMakeWire;
		occtMakeWire.Add(occtEdges);

		try {
			return new Wire(occtMakeWire);
		}
		catch (StdFail_NotDone&)
		{
			throw new std::exception(GetOcctMakeWireErrorMessage(occtMakeWire).c_str());
		}
	}

	/*Wire * Wire::ByPolyCurve(TopoDS_Wire const * const kpkWire)
	{
		return nullptr;
	}*/

	void Wire::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		// Returns a list of curves
		std::list<Edge*> edges;
		Edges(edges);

		for (std::list<Edge*>::const_iterator kEdgeIterator = edges.begin();
			kEdgeIterator != edges.end();
			kEdgeIterator++)
		{
			rOcctGeometries.push_back((*kEdgeIterator)->Curve());
		}
	}

	TopoDS_Shape* Wire::GetOcctShape() const
	{
		assert(m_pOcctWire != nullptr && "Wire::m_pOcctWire is null.");
		if (m_pOcctWire == nullptr)
		{
			throw std::exception("Wire::m_pOcctWire is null.");
		}

		return m_pOcctWire;
	}

	std::string Wire::GetOcctMakeWireErrorMessage(const BRepBuilderAPI_MakeWire & rkOcctMakeWire)
	{
		// The error messages are based on those in the OCCT documentation.
		// https://www.opencascade.com/doc/occt-7.1.0/refman/html/_b_rep_builder_a_p_i___wire_error_8hxx.html

		switch (rkOcctMakeWire.Error())
		{
		case BRepBuilderAPI_EmptyWire:
			return std::string("No initialization of the algorithm. Only an empty constructor was used.");

		case BRepBuilderAPI_DisconnectedWire:
			return std::string("The last edge which you attempted to add was not connected to the wire.");

		case BRepBuilderAPI_NonManifoldWire:
			return std::string("The wire has some singularity.");

		default: // i.e. BRepBuilderAPI_WireDone 
			return std::string("A wire was successfully created.");
		}
	}

	Wire::Wire(const TopoDS_Wire& rkOcctWire)
		: Topology(1)
		, m_pOcctWire(nullptr)
	{
		ShapeFix_Wire occtFixWire;
		occtFixWire.Load(rkOcctWire);
		occtFixWire.Perform();
		m_pOcctWire = new TopoDS_Wire(occtFixWire.Wire());
		GlobalCluster::GetInstance().Add(this);
	}

	Wire::~Wire()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctWire;
	}
}
