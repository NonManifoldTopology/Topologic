#include <Vertex.h>
#include <GlobalCluster.h>
#include <Edge.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS_Vertex.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	Vertex::Vertex(const TopoDS_Vertex& rkOcctVertex)
		: Topology(0)
		, m_occtVertex(rkOcctVertex)
	{
		GlobalCluster::GetInstance().Add(this);
	}

	Vertex::~Vertex()
	{
		GlobalCluster::GetInstance().Remove(this);
	}

	std::shared_ptr<Vertex> Vertex::ByPoint(Handle(Geom_Point) pOcctPoint)
	{
		return std::make_shared<Vertex>(BRepBuilderAPI_MakeVertex(pOcctPoint->Pnt()));
	}

	void Vertex::Edges(std::list<std::shared_ptr<Edge>>& rEdges)
	{
		UpwardNavigation(rEdges);
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
		assert(m_occtVertex.IsNull() && "Vertex::m_occtVertex is null.");
		if (m_occtVertex.IsNull())
		{
			throw std::exception("Vertex::m_occtVertex is null.");
		}

		return m_occtVertex;
	}

	const TopoDS_Vertex& Vertex::GetOcctVertex() const
	{
		assert(m_occtVertex.IsNull() && "Vertex::m_occtVertex is null.");
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