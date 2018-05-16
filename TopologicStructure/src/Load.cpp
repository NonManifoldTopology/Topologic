#include <Load.h>

namespace TopologicStructure
{
	Load^ Load::ByVertex(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector)
	{
		return gcnew Load(vertex, TopologicSupport::Vector::ByCoordinates(vector->X, vector->Y, vector->Z));
	}

	Load^ Load::ByEdge(Topologic::Edge^ edge, double u, Autodesk::DesignScript::Geometry::Vector^ vector)
	{
		Topologic::Vertex^ pVertex = edge->PointAtParameter(u);
		return gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(vector->X, vector->Y, vector->Z));
	}

	Load^ Load::ByFace(Topologic::Face^ face, double u, double v, Autodesk::DesignScript::Geometry::Vector^ vector)
	{
		Autodesk::DesignScript::Geometry::UV^ pUV = Autodesk::DesignScript::Geometry::UV::ByCoordinates(u, v);
		Topologic::Vertex^ pVertex = face->PointAtParameter(pUV);
		
		delete pUV;

		if (vector != nullptr)
		{
			return gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(vector->X, vector->Y, vector->Z));
		}

		// If vector is null, use the reverse surface normal
		Autodesk::DesignScript::Geometry::Vector^ pSurfaceNormal = face->NormalAtParameter(pUV);
		Load^ pLoad = gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(-pSurfaceNormal->X, -pSurfaceNormal->Y, -pSurfaceNormal->Z));
		delete pSurfaceNormal;
		return pLoad;
	}

	double Load::Magnitude::get()
	{
		return (*m_pVector)->Magnitude();
	}

	Load::Load(Topologic::Vertex^ vertex, const TopologicSupport::Vector::Ptr& vector)
		: Topologic::Vertex(vertex)
		, m_pVector(new TopologicSupport::Vector::Ptr(vector))
	{

	}

	Load::~Load()
	{
		delete m_pVector;
	}

	Autodesk::DesignScript::Geometry::Vector^ Load::Direction::get()
	{
		TopologicSupport::Vector::Ptr pNormalizedVector = TopologicSupport::Vector::ByNormalizedVector(*m_pVector);
		return Autodesk::DesignScript::Geometry::Vector::ByCoordinates(pNormalizedVector->X(), pNormalizedVector->Y(), pNormalizedVector->Z());
	}

	Object^ Load::Geometry::get()
	{
		// Draw a cylinder and a cone
		Autodesk::DesignScript::Geometry::Point^ pLoadEndPoint = Point();
		TopologicSupport::Vector::Ptr pReverseVector = TopologicSupport::Vector::ByReverseVector(*m_pVector);
		Autodesk::DesignScript::Geometry::Vector^ pDynamoReverseVector = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(pReverseVector->X(), pReverseVector->Y(), pReverseVector->Z());
		TopologicSupport::Vector::Ptr pScaledReverseVector = TopologicSupport::Vector::ByScaledVector(pReverseVector, 0.1);
		Autodesk::DesignScript::Geometry::Vector^ pDynamoScaledReverseVector = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(pScaledReverseVector->X(), pScaledReverseVector->Y(), pScaledReverseVector->Z());
		Autodesk::DesignScript::Geometry::Point^ pConeStartPoint = safe_cast<Autodesk::DesignScript::Geometry::Point^>(pLoadEndPoint->Translate(pDynamoScaledReverseVector));
		Autodesk::DesignScript::Geometry::Point^ pCylinderStartPoint = safe_cast<Autodesk::DesignScript::Geometry::Point^>(pLoadEndPoint->Translate(pDynamoReverseVector));
		Autodesk::DesignScript::Geometry::Cylinder^ pLoadCylinder = Autodesk::DesignScript::Geometry::Cylinder::ByPointsRadius(pCylinderStartPoint, pConeStartPoint, 0.05);
		Autodesk::DesignScript::Geometry::Cone^ pLoadCone = Autodesk::DesignScript::Geometry::Cone::ByPointsRadius(pConeStartPoint, pLoadEndPoint, 0.1);

		delete pCylinderStartPoint;
		delete pConeStartPoint;
		delete pDynamoReverseVector;
		delete pDynamoScaledReverseVector;
		delete pLoadEndPoint;

		List<Object^>^ pLoadGeometry = gcnew List<Object^>();
		pLoadGeometry->Add(pLoadCone);
		pLoadGeometry->Add(pLoadCylinder);

		return pLoadGeometry;
	}
}