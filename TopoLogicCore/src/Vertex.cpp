#include <Vertex.h>
#include <Edge.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS_Vertex.hxx>

#include <assert.h>

namespace TopologicCore
{
	Vertex::Vertex(const TopoDS_Vertex& rkOcctVertex, const std::string& rkGuid)
		: Topology(0, rkOcctVertex, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtVertex(rkOcctVertex)
	{

	}

	Vertex::~Vertex()
	{

	}

	Vertex::Ptr Vertex::ByPoint(Handle(Geom_Point) pOcctPoint)
	{
		return std::make_shared<Vertex>(BRepBuilderAPI_MakeVertex(pOcctPoint->Pnt()));
	}

	void Vertex::Edges(const Topology::Ptr& kpParentTopology, std::list<Edge::Ptr>& rEdges)
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

	std::shared_ptr<Vertex> Vertex::CenterOfMass() const
	{
		Handle(Geom_Point) pOcctPoint = Point();
		return ByPoint(pOcctPoint);
	}
	std::string Vertex::GetTypeAsString() const
	{
		return std::string("Vertex");
	}
}