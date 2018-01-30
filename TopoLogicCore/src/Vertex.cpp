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
	Vertex::Vertex(TopoDS_Vertex * const kpOcctVertex)
		: Topology(0)
		, m_pOcctVertex(kpOcctVertex)
	{
		GlobalCluster::GetInstance().Add(this);
	}

	Vertex::~Vertex()
	{
		GlobalCluster::GetInstance().Remove(this);
		delete m_pOcctVertex;
	}

	Vertex * Vertex::ByPoint(Handle(Geom_Point) pOcctPoint)
	{
		return new Vertex(new TopoDS_Vertex(BRepBuilderAPI_MakeVertex(pOcctPoint->Pnt())));
	}

	void Vertex::Edges(std::list<Edge*>& rEdges)
	{
		TopTools_IndexedDataMapOfShapeListOfShape occtVertexToEdgesMap;
		TopExp::MapShapesAndUniqueAncestors(*GlobalCluster::GetInstance().GetCluster()->GetOcctShape(), TopAbs_VERTEX, TopAbs_EDGE, occtVertexToEdgesMap);

		const TopTools_ListOfShape& rkOcctEdges = occtVertexToEdgesMap.FindFromKey(*GetOcctShape());

		for (TopTools_ListOfShape::const_iterator kIterator = rkOcctEdges.cbegin();
			kIterator != rkOcctEdges.cend();
			kIterator++)
		{
			if (kIterator->ShapeType() == TopAbs_EDGE)
			{
				const TopoDS_Edge& pOcctEdge = TopoDS::Edge(*kIterator);
				rEdges.push_back(new Edge(new TopoDS_Edge(pOcctEdge)));
			}
		}
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