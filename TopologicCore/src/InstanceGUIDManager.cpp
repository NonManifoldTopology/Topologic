#include <InstanceGUIDManager.h>

#include <Topology.h>

namespace TopologicCore
{
	void InstanceGUIDManager::Add(const TopoDS_Shape& rkOcctShape, const std::string& rkGUID)
	{
		m_occtShapeToGUIDMap[rkOcctShape] = rkGUID;
	}

	void InstanceGUIDManager::Remove(const TopoDS_Shape & rkOcctShape)
	{
		m_occtShapeToGUIDMap.erase(rkOcctShape);
	}

	bool InstanceGUIDManager::Find(const TopoDS_Shape& rkOcctShape, std::string& rkGUID)
	{
		if (m_occtShapeToGUIDMap.find(rkOcctShape) != m_occtShapeToGUIDMap.end())
		{
			rkGUID = m_occtShapeToGUIDMap[rkOcctShape];
			return true;
		}

		return false;
	}

	void InstanceGUIDManager::ClearAll()
	{
		m_occtShapeToGUIDMap.clear();
	}
}