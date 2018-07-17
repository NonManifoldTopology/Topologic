#include <Load.h>
#include <OcctLoadAttribute.h>

#include <TopologicCore/include/Vertex.h>

#include <assert.h>

namespace TopologicStructure
{
	Load^ Load::ByVertex(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector, double magnitude)
	{
		Autodesk::DesignScript::Geometry::Vector^ pNormalizedVector = vector->Normalized();
		Autodesk::DesignScript::Geometry::Vector^ pScaledVector = pNormalizedVector->Scale(magnitude);
		Load^ pLoad = gcnew Load(vertex, TopologicSupport::Vector::ByCoordinates(pScaledVector->X, pScaledVector->Y, pScaledVector->Z), true);
		delete pNormalizedVector;
		delete pScaledVector;
		return pLoad;
	}

	Load^ Load::ByEdge(Topologic::Edge^ edge, double u, Autodesk::DesignScript::Geometry::Vector^ vector, double magnitude)
	{
		if (u < 0.0 || u > 1.0)
		{
			throw gcnew Exception("Parameter out of bound; it needs to be between 0 and 1.");
		}

		Autodesk::DesignScript::Geometry::Vector^ pNormalizedVector = vector->Normalized();
		Autodesk::DesignScript::Geometry::Vector^ pScaledVector = pNormalizedVector->Scale(magnitude);
		Topologic::Vertex^ pVertex = edge->PointAtParameter(u);
		Load^ pLoad = gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(pScaledVector->X, pScaledVector->Y, pScaledVector->Z), true);
		delete pNormalizedVector;
		delete pScaledVector;

		return pLoad;
		//return gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(vector->X, vector->Y, vector->Z), true);
	}

	Load^ Load::ByFace(Topologic::Face^ face, double u, double v, Autodesk::DesignScript::Geometry::Vector^ vector, bool reverseDefaultNormal, double magnitude)
	{
		Autodesk::DesignScript::Geometry::UV^ pUV = Autodesk::DesignScript::Geometry::UV::ByCoordinates(u, v);
		try {
			Topologic::Vertex^ pVertex = face->PointAtParameter(pUV);

			delete pUV;

			if (vector != nullptr)
			{
				//return gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(vector->X, vector->Y, vector->Z), true);

				Autodesk::DesignScript::Geometry::Vector^ pNormalizedVector = vector->Normalized();
				Autodesk::DesignScript::Geometry::Vector^ pScaledVector = pNormalizedVector->Scale(magnitude);
				Load^ pLoad = gcnew Load(pVertex, TopologicSupport::Vector::ByCoordinates(pScaledVector->X, pScaledVector->Y, pScaledVector->Z), true);
				delete pNormalizedVector;
				delete pScaledVector;

				return pLoad;
			}

			// If vector is null, use the reverse surface normal
			Autodesk::DesignScript::Geometry::Vector^ pSurfaceNormal = face->NormalAtParameter(pUV);

			// Default: already reversed
			double xDirection = pSurfaceNormal->X;
			double yDirection = pSurfaceNormal->Y;
			double zDirection = pSurfaceNormal->Z;
			if (!reverseDefaultNormal)
			{
				xDirection = -xDirection;
				yDirection = -yDirection;
				zDirection = -zDirection;
			}
			TopologicSupport::Vector::Ptr pVector = TopologicSupport::Vector::ByCoordinates(xDirection, yDirection, zDirection);

			TopologicSupport::Vector::Ptr pScaledVector = TopologicSupport::Vector::ByScaledVector(pVector, magnitude);
			Load^ pLoad = gcnew Load(pVertex, pScaledVector, true);
			delete pSurfaceNormal;
			return pLoad;
		}
		catch (Exception^)
		{
			throw gcnew Exception("Load not on the face.");
		}
	}

	double Load::Magnitude::get()
	{
		return (*m_pVector)->Magnitude();
	}

	//void Load::AttachAttribute()
	//{
	//	//GetCoreTopologicalQuery();
	//}

	Load::Load(Topologic::Vertex^ vertex, const TopologicSupport::Vector::Ptr& vector, bool attachAttribute)
		: Topologic::Vertex(vertex)
		, m_pVector(new TopologicSupport::Vector::Ptr(vector))
		, m_pTopology(vertex)
	{
		Handle(OcctLoadAttribute) pOcctLoadAttribute = new OcctLoadAttribute(
			vector->X(), vector->Y(), vector->Z(),
			vector->Magnitude());
		
		//if (attachAttribute)
		//{
		//	// Attach the load attribute to the vertex's label
		//	AttachAttribute(pOcctLoadAttribute.get());
		//}
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
		double distance = pConeStartPoint->DistanceTo(pCylinderStartPoint);
		Autodesk::DesignScript::Geometry::Cylinder^ pLoadCylinder = Autodesk::DesignScript::Geometry::Cylinder::ByPointsRadius(pCylinderStartPoint, pConeStartPoint, 0.05 * distance);
		Autodesk::DesignScript::Geometry::Cone^ pLoadCone = Autodesk::DesignScript::Geometry::Cone::ByPointsRadius(pConeStartPoint, pLoadEndPoint, 0.1 * distance);

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