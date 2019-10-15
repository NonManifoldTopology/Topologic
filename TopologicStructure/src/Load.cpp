// This file is part of Topologic software library.
// Copyright(C) 2019, Cardiff University and University College London
//
// This program is free software : you can redistribute it and/or modify it
// under the terms of the GNU Affero General Public License version 3 (AGPL v3)
// as published by the Free Software Foundation.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
//
// Please consult the file LICENSE.txt included in Topologic distribution
// for complete text of the license and disclaimer of any warranty.
// Alternatively, please see https://www.gnu.org/licenses/agpl-3.0.en.html.

//#include <OcctLoadAttribute.h>
#include <Load.h>
#include <LoadFactory.h>

//#include <TopologicCore/include/Vertex.h>

#include <assert.h>

namespace TopologicStructure
{
	Load^ Load::ByVertex(Topologic::Vertex^ vertex, Autodesk::DesignScript::Geometry::Vector^ vector, double magnitude)
	{
		Autodesk::DesignScript::Geometry::Vector^ pNormalizedVector = vector->Normalized();
		Autodesk::DesignScript::Geometry::Vector^ pScaledVector = pNormalizedVector->Scale(magnitude);
		Load^ pLoad = gcnew Load(vertex, TopologicUtility::Vector::ByCoordinates(pScaledVector->X, pScaledVector->Y, pScaledVector->Z), true);
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
		Load^ pLoad = gcnew Load(pVertex, TopologicUtility::Vector::ByCoordinates(pScaledVector->X, pScaledVector->Y, pScaledVector->Z), true);
		delete pNormalizedVector;
		delete pScaledVector;

		return pLoad;
		//return gcnew Load(pVertex, TopologicUtility::Vector::ByCoordinates(vector->X, vector->Y, vector->Z), true);
	}

	Load^ Load::ByFace(Topologic::Face^ face, double u, double v, Autodesk::DesignScript::Geometry::Vector^ vector, bool reverseDefaultNormal, double magnitude)
	{
		Autodesk::DesignScript::Geometry::UV^ pUV = Autodesk::DesignScript::Geometry::UV::ByCoordinates(u, v);
		try {
			Topologic::Vertex^ pVertex = face->PointAtParameter(pUV);

			delete pUV;

			if (vector != nullptr)
			{
				//return gcnew Load(pVertex, TopologicUtility::Vector::ByCoordinates(vector->X, vector->Y, vector->Z), true);

				Autodesk::DesignScript::Geometry::Vector^ pNormalizedVector = vector->Normalized();
				Autodesk::DesignScript::Geometry::Vector^ pScaledVector = pNormalizedVector->Scale(magnitude);
				Load^ pLoad = gcnew Load(pVertex, TopologicUtility::Vector::ByCoordinates(pScaledVector->X, pScaledVector->Y, pScaledVector->Z), true);
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
			TopologicUtility::Vector::Ptr pVector = TopologicUtility::Vector::ByCoordinates(xDirection, yDirection, zDirection);

			TopologicUtility::Vector::Ptr pScaledVector = TopologicUtility::Vector::ByScaledVector(pVector, magnitude);
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

	Load::Load(Topologic::Vertex^ vertex, const TopologicUtility::Vector::Ptr& vector, bool attachAttribute)
		: Topologic::Vertex(vertex)
		, m_pVector(new TopologicUtility::Vector::Ptr(vector))
		, m_pTopology(vertex)
	{
		RegisterFactory(LoadFactory::GetGUID(), gcnew LoadFactory());

		/*Handle(OcctLoadAttribute) pOcctLoadAttribute = new OcctLoadAttribute(
			vector->X(), vector->Y(), vector->Z(),
			vector->Magnitude());*/
		
		//if (attachAttribute)
		//{
		//	// Attach the load attribute to the vertex's label
		//	AttachAttribute(pOcctLoadAttribute.get());
		//}
	}

	Load::Load(const TopologicCore::Vertex::Ptr& vertex)
		: Topologic::Vertex(vertex)
		, m_pVector(nullptr)
		, m_pTopology(nullptr)
	{
		RegisterFactory(LoadFactory::GetGUID(), gcnew LoadFactory());
	}

	Load::~Load()
	{
		delete m_pVector;
	}

	Autodesk::DesignScript::Geometry::Vector^ Load::Direction::get()
	{
		TopologicUtility::Vector::Ptr pNormalizedVector = TopologicUtility::Vector::ByNormalizedVector(*m_pVector);
		return Autodesk::DesignScript::Geometry::Vector::ByCoordinates(pNormalizedVector->X(), pNormalizedVector->Y(), pNormalizedVector->Z());
	}

	Object^ Load::Geometry::get()
	{
		// Draw a cylinder and a cone
		Autodesk::DesignScript::Geometry::Point^ pLoadEndPoint = Point();
		TopologicUtility::Vector::Ptr pReverseVector = TopologicUtility::Vector::ByReverseVector(*m_pVector);
		Autodesk::DesignScript::Geometry::Vector^ pDynamoReverseVector = Autodesk::DesignScript::Geometry::Vector::ByCoordinates(pReverseVector->X(), pReverseVector->Y(), pReverseVector->Z());
		TopologicUtility::Vector::Ptr pScaledReverseVector = TopologicUtility::Vector::ByScaledVector(pReverseVector, 0.1);
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