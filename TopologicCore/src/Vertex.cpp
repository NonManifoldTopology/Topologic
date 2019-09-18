#include "Vertex.h"
#include "Edge.h"
#include "VertexFactory.h"
#include "GlobalCluster.h"

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS.hxx>
#include <TopoDS_Vertex.hxx>

#include <assert.h>

namespace TopologicCore
{
	Vertex::Vertex(const TopoDS_Vertex& rkOcctVertex, const std::string& rkGuid)
		: Topology(0, rkOcctVertex, rkGuid.compare("") == 0 ? GetClassGUID() : rkGuid)
		, m_occtVertex(rkOcctVertex)
	{
		RegisterFactory(GetClassGUID(), std::make_shared<VertexFactory>());
	}

	Vertex::~Vertex()
	{

	}

	Vertex::Ptr Vertex::ByPoint(Handle(Geom_Point) pOcctPoint)
	{
		Vertex::Ptr pVertex = std::make_shared<Vertex>(BRepBuilderAPI_MakeVertex(pOcctPoint->Pnt()));
		GlobalCluster::GetInstance().AddTopology(pVertex->GetOcctVertex());
		return pVertex;
	}

	Vertex::Ptr Vertex::ByCoordinates(const double kX, const double kY, const double kZ)
	{
		Vertex::Ptr pVertex = std::make_shared<Vertex>(BRepBuilderAPI_MakeVertex(gp_Pnt(kX, kY, kZ)));
		GlobalCluster::GetInstance().AddTopology(pVertex->GetOcctVertex());
		return pVertex;
	}

	void Vertex::Edges(std::list<Edge::Ptr>& rEdges)
	{
		UpwardNavigation(rEdges);
	}

	bool Vertex::IsManifold() const
	{
		throw std::exception("Not implemented yet");
	}

	void Vertex::Geometry(std::list<Handle(Geom_Geometry)>& rOcctGeometries) const
	{
		rOcctGeometries.push_back(Point());
	}

	TopoDS_Shape& Vertex::GetOcctShape()
	{
		return GetOcctVertex();
	}

	void Vertex::SetOcctShape(const TopoDS_Shape & rkOcctShape)
	{
		try {
			SetOcctVertex(TopoDS::Vertex(rkOcctShape));
		}
		catch (Standard_Failure e)
		{
			throw std::exception(e.GetMessageString());
		}
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
			throw std::exception("A null Vertex is encountered.");
		}

		return m_occtVertex;
	}

	void Vertex::SetOcctVertex(const TopoDS_Vertex & rkOcctVertex)
	{
		m_occtVertex = rkOcctVertex;
	}

	const TopoDS_Vertex& Vertex::GetOcctVertex() const
	{
		assert(!m_occtVertex.IsNull() && "Vertex::m_occtVertex is null.");
		if (m_occtVertex.IsNull())
		{
			throw std::exception("A null Vertex is encountered.");
		}

		return m_occtVertex;
	}

	Handle(Geom_Point) Vertex::Point() const
	{
		return new Geom_CartesianPoint(BRep_Tool::Pnt(GetOcctVertex()));
	}

	double Vertex::X() const
	{
		Handle(Geom_Point) pOcctPoint = Point();
		return pOcctPoint->X();
	}

	double Vertex::Y() const
	{
		Handle(Geom_Point) pOcctPoint = Point();
		return pOcctPoint->Y();
	}

	double Vertex::Z() const
	{
		Handle(Geom_Point) pOcctPoint = Point();
		return pOcctPoint->Z();
	}

	std::tuple<double, double, double> Vertex::Coordinates() const
	{
		Handle(Geom_Point) pOcctPoint = Point();
		return std::tuple<double, double, double>(pOcctPoint->X(), pOcctPoint->Y(), pOcctPoint->Z());
	}

	std::shared_ptr<Vertex> Vertex::CenterOfMass() const
	{
		TopoDS_Vertex occtCenterOfMass = CenterOfMass(GetOcctVertex());
		return std::dynamic_pointer_cast<Vertex>(Topology::ByOcctShape(occtCenterOfMass));
		/*Handle(Geom_Point) pOcctPoint = Point();
		return ByPoint(pOcctPoint);*/
	}
	TopoDS_Vertex Vertex::CenterOfMass(const TopoDS_Vertex & rkOcctVertex)
	{
		return BRepBuilderAPI_MakeVertex(BRep_Tool::Pnt(rkOcctVertex));
	}
	std::string Vertex::GetTypeAsString() const
	{
		return std::string("Vertex");
	}
}