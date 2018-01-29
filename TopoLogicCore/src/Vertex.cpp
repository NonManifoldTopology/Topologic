#include <Vertex.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>

#include <assert.h>

namespace TopoLogicCore
{
	Vertex::Vertex(TopoDS_Vertex * const kpOcctVertex)
		: Topology(0)
		, m_pOcctVertex(kpOcctVertex)
	{
	}
	Vertex::~Vertex()
	{
		delete m_pOcctVertex;
	}
	Vertex * Vertex::ByPoint(Handle(Geom_Point) pOcctPoint)
	{
		return new Vertex(new TopoDS_Vertex(BRepBuilderAPI_MakeVertex(pOcctPoint->Pnt())));
	}
	void Vertex::Edges(std::list<Edge*>& rEdges)
	{
	}

	void Vertex::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		assert(m_pOcctVertex != nullptr && "Vertex::m_pOcctVertex is null.");
		if (m_pOcctVertex == nullptr)
		{
			throw std::exception("Vertex::m_pOcctVertex is null.");
		}
		
		rOcctGeometries.push_back(new Geom_CartesianPoint(BRep_Tool::Pnt(TopoDS::Vertex(*m_pOcctVertex))));
	}
}