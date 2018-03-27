#include <Vertex.h>
#include <GlobalCluster.h>
#include <Edge.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopExp.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>
#include <TopoDS_Edge.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	Vertex::Vertex(const TopoDS_Vertex& rkOcctVertex)
		: Topology(0)
		, m_pOcctVertex(std::make_shared<TopoDS_Vertex>(rkOcctVertex))
	{
		GlobalCluster::GetInstance().Add(shared_from_this());
	}

	Vertex::~Vertex()
	{
		GlobalCluster::GetInstance().Remove(shared_from_this());
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

	std::shared_ptr<TopoDS_Shape> Vertex::GetOcctShape() const
	{
		assert(m_pOcctVertex != nullptr && "Vertex::m_pOcctVertex is null.");
		if (m_pOcctVertex == nullptr)
		{
			throw std::exception("Vertex::m_pOcctVertex is null.");
		}

		return m_pOcctVertex;
	}

	Handle(Geom_Point) Vertex::Point() const
	{
		return new Geom_CartesianPoint(BRep_Tool::Pnt(TopoDS::Vertex(*GetOcctShape())));
	}
}