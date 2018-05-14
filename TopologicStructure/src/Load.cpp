#include <Load.h>


namespace TopologicStructure
{
	Load^ Load::ByVertex(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector)
	{
		return gcnew Load(vertex,vector);
	}

	Load^ Load::ByEdge(Topologic::Edge^ edge, double u, Autodesk::DesignScript::Geometry::Vector^ vector)
	{
		Topologic::Vertex^ pVertex = edge->PointAtParameter(u);
		return gcnew Load(pVertex, vector);
	}

	Load^ Load::ByFace(Topologic::Face^ face, double u, double v, Autodesk::DesignScript::Geometry::Vector^ vector)
	{
		Autodesk::DesignScript::Geometry::UV^ pUV = Autodesk::DesignScript::Geometry::UV::ByCoordinates(u, v);
		Topologic::Vertex^ pVertex = face->PointAtParameter(pUV);
		
		delete pUV;
		
		return gcnew Load(pVertex, vector);
	}

	double Load::Magnitude::get()
	{
		return m_pVector->Length;
	}

	Load::Load(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector)
		: Topologic::Vertex(vertex)
		, m_pVector(vector)
	{

	}

	Load::~Load()
	{
		// Do not delete vector because it is the same as the input, Dynamo takes care of it.
	}

	Object^ Load::Geometry::get()
	{
		// Draw a cylinder and a cone
		Autodesk::DesignScript::Geometry::Point^ pLoadEndPoint = Point();
		Autodesk::DesignScript::Geometry::Vector^ pReverseVector = m_pVector->Reverse();
		Autodesk::DesignScript::Geometry::Vector^ pScaledReverseVector = pReverseVector->Scale(0.1);
		Autodesk::DesignScript::Geometry::Point^ pConeStartPoint = safe_cast<Autodesk::DesignScript::Geometry::Point^>(pLoadEndPoint->Translate(pScaledReverseVector));
		Autodesk::DesignScript::Geometry::Point^ pCylinderStartPoint = safe_cast<Autodesk::DesignScript::Geometry::Point^>(pLoadEndPoint->Translate(pReverseVector));
		Autodesk::DesignScript::Geometry::Cylinder^ pLoadCylinder = Autodesk::DesignScript::Geometry::Cylinder::ByPointsRadius(pCylinderStartPoint, pConeStartPoint, 0.05);
		Autodesk::DesignScript::Geometry::Cone^ pLoadCone = Autodesk::DesignScript::Geometry::Cone::ByPointsRadius(pConeStartPoint, pLoadEndPoint, 0.1);

		delete pCylinderStartPoint;
		delete pConeStartPoint;
		delete pReverseVector;
		delete pScaledReverseVector;
		delete pLoadEndPoint;

		List<Object^>^ pLoadGeometry = gcnew List<Object^>();
		pLoadGeometry->Add(pLoadCone);
		pLoadGeometry->Add(pLoadCylinder);

		return pLoadGeometry;
	}
}