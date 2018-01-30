#include <Vertex.h>

#include <BRepBuilderAPI_MakeVertex.hxx>
#include <BRep_Tool.hxx>
#include <Geom_CartesianPoint.hxx>
#include <TopoDS.hxx>

namespace TopoLogic
{
	Dictionary<String^, Object^>^ Vertex::ByPoint(Autodesk::DesignScript::Geometry::Point ^ point)
	{
		Dictionary<String^, Object^>^ pDictionary = gcnew Dictionary<String^, Object^>();
		try {
			Vertex^ pVertex = gcnew Vertex(point);
			pDictionary->Add("TopoLogic Vertex", pVertex);
			pDictionary->Add("Point", pVertex->Geometry);
		}
		catch (ArgumentNullException^)
		{
			throw gcnew ArgumentNullException("point");
		}
		return pDictionary;
	}

	Dictionary<String^, Object^>^ Vertex::Edges(Vertex ^ topoLogicVertex)
	{
		throw gcnew System::NotImplementedException();
		// TODO: insert return statement here
	}

	Vertex::Vertex(Autodesk::DesignScript::Geometry::Point ^ pDynamoPoint)
		: m_pCoreVertex(TopoLogicCore::Vertex::ByPoint(new Geom_CartesianPoint(gp_Pnt(pDynamoPoint->X, pDynamoPoint->Y, pDynamoPoint->Z))))
	{

	}

	Vertex::~Vertex()
	{
		delete m_pCoreVertex;
	}

	Object^ Vertex::Geometry::get()
	{
		gp_Pnt occtPoint = BRep_Tool::Pnt(TopoDS::Vertex(*m_pCoreVertex->GetOcctShape()));
		return Autodesk::DesignScript::Geometry::Point::ByCoordinates(occtPoint.X(), occtPoint.Y(), occtPoint.Z());
	}
}