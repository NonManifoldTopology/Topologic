#include <AttributeManager.h>
#include <Attribute.h>

namespace TopologicCore
{
	void AttributeManager::Add(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<Attribute>& kpAttribute)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) == m_occtShapeToAttributesMap.end())
		{
			std::list<Attribute::Ptr> attributes;
			m_occtShapeToAttributesMap.insert(std::pair<TopoDS_Shape, std::list<std::shared_ptr<Attribute>>>(rkOcctShape, attributes));
		}

		m_occtShapeToAttributesMap[rkOcctShape].push_back(kpAttribute);
	}

	void AttributeManager::Remove(const TopoDS_Shape & rkOcctShape, const std::shared_ptr<Attribute>& kpAttribute)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap[rkOcctShape].remove(kpAttribute);
		}
	}

	bool AttributeManager::Find(const TopoDS_Shape& rkOcctShape, std::list<std::shared_ptr<Attribute>>& rAttributes)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			std::list<Attribute::Ptr> attributes = m_occtShapeToAttributesMap[rkOcctShape];
			rAttributes.insert(rAttributes.end(), attributes.begin(), attributes.end());
			return true;
		}

		return false;
	}

	void AttributeManager::ClearOne(const TopoDS_Shape & rkOcctShape)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap[rkOcctShape].clear();
			m_occtShapeToAttributesMap.erase(rkOcctShape);
		}
	}

	void AttributeManager::ClearAll()
	{
		m_occtShapeToAttributesMap.clear();
	}
}