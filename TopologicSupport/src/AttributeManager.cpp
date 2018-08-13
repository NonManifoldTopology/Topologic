#include <AttributeManager.h>
#include <AttributeMap.h>
#include <Attribute.h>

namespace TopologicSupport
{
	void AttributeManager::Add(const TopoDS_Shape& rkOcctShape, const std::string& rkKey, const Attribute::Ptr& kpAttribute)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) == m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap.insert(std::pair<TopoDS_Shape, AttributeMap::Ptr>(rkOcctShape, std::make_shared<AttributeMap>()));
		}

		m_occtShapeToAttributesMap[rkOcctShape]->Add(rkKey, kpAttribute);
	}

	void AttributeManager::Remove(const TopoDS_Shape & rkOcctShape, const std::string& rkKey)
	{
		m_occtShapeToAttributesMap[rkOcctShape]->Remove(rkKey);
	}

	bool AttributeManager::Find(const TopoDS_Shape& rkOcctShape, const std::string& rkKey, std::shared_ptr<Attribute>& rAttributes)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			return m_occtShapeToAttributesMap[rkOcctShape]->Find(rkKey, rAttributes);
		}

		return false;
	}

	void AttributeManager::ClearOne(const TopoDS_Shape & rkOcctShape)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap[rkOcctShape]->Clear();
		}
	}

	void AttributeManager::ClearAll()
	{
		m_occtShapeToAttributesMap.clear();
	}
}