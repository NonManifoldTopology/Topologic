#include <Vertex.h>
//#include <GlobalCluster.h>
#include <Edge.h>
#include <OcctCounterAttribute.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS_Vertex.hxx>

#include <assert.h>

#include <LabelManager.h>

namespace TopoLogicCore
{
	Vertex::Vertex(const TopoDS_Vertex& rkOcctVertex, const TDF_Label& rkOcctLabel)
		: Topology(0)
		, m_occtVertex(rkOcctVertex)
	{
		//GlobalCluster::GetInstance().Add(this);
		
		// Needs to be done in the subclass, not in Topology, as the OCCT shape is not yet defined there.
		SetOcctLabel(rkOcctLabel);
		OcctCounterAttribute::IncreaseCounter(GetOcctLabel());
	}

	Vertex::~Vertex()
	{
		//GlobalCluster::GetInstance().Remove(this);
		DecreaseCounter();
	}

	std::shared_ptr<Vertex> Vertex::ByPoint(Handle(Geom_Point) pOcctPoint)
	{
		return std::make_shared<Vertex>(BRepBuilderAPI_MakeVertex(pOcctPoint->Pnt()));
	}

	void Vertex::Edges(const std::shared_ptr<Topology>& kpParentTopology, std::list<std::shared_ptr<Edge>>& rEdges)
	{
		UpwardNavigation(kpParentTopology, rEdges);
	}

	void Vertex::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		rOcctGeometries.push_back(Point());
	}

	TopoDS_Shape& Vertex::GetOcctShape()
	{
		return GetOcctVertex();
	}

	const TopoDS_Shape& Vertex::GetOcctShape() const
	{
		return GetOcctVertex();
	}

	TopoDS_Vertex& Vertex::GetOcctVertex()
	{
		assert(!m_occtVertex.IsNull() && "Vertex::m_occtVertex is null.");
		if (m_occtVertex.IsNull())
		{
			throw std::exception("Vertex::m_occtVertex is null.");
		}

		return m_occtVertex;
	}

	const TopoDS_Vertex& Vertex::GetOcctVertex() const
	{
		assert(!m_occtVertex.IsNull() && "Vertex::m_occtVertex is null.");
		if (m_occtVertex.IsNull())
		{
			throw std::exception("Vertex::m_occtVertex is null.");
		}

		return m_occtVertex;
	}

	Handle(Geom_Point) Vertex::Point() const
	{
		return new Geom_CartesianPoint(BRep_Tool::Pnt(GetOcctVertex()));
	}
}