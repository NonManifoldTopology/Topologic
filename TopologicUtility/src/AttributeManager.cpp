#include <AttributeManager.h>
#include <Attribute.h>

#include <TopologicCore/include/Topology.h>

namespace TopologicUtility
{
	void AttributeManager::Add(const std::shared_ptr<TopologicCore::Topology>& kpTopology, const std::string & kAttributeName, const std::shared_ptr<Attribute>& kpAttribute)
	{
		Add(kpTopology->GetOcctShape(), kAttributeName, kpAttribute);
	}

	void AttributeManager::Add(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName, const std::shared_ptr<Attribute>& kpAttribute)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) == m_occtShapeToAttributesMap.end())
		{
			std::map<std::string, Attribute::Ptr> attributeMap;
			m_occtShapeToAttributesMap.insert(std::pair<TopoDS_Shape, std::map<std::string, std::shared_ptr<Attribute>>>(rkOcctShape, attributeMap));
		}

		m_occtShapeToAttributesMap[rkOcctShape][kAttributeName] = kpAttribute;
	}

	void AttributeManager::Remove(const TopologicCore::Topology::Ptr& kpTopology, const std::string& kAttributeName)
	{
		Remove(kpTopology->GetOcctShape(), kAttributeName);
	}

	void AttributeManager::Remove(const TopoDS_Shape& rkOcctShape, const std::string& kAttributeName)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap[rkOcctShape].erase(kAttributeName);
		}
	}

	Attribute::Ptr AttributeManager::Find(const TopoDS_Shape& rkOcctShape, const std::string& rkAttributeName)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			std::map<std::string, Attribute::Ptr>& rkAttributeMap = m_occtShapeToAttributesMap[rkOcctShape];
			if (rkAttributeMap.find(rkAttributeName) != rkAttributeMap.end())
			{
				return rkAttributeMap[rkAttributeName];
			}
			return nullptr;
		}

		return nullptr;
	}

	bool AttributeManager::FindAll(const TopoDS_Shape & rkOcctShape, std::map<std::string, std::shared_ptr<Attribute>>& rAttributes)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) != m_occtShapeToAttributesMap.end())
		{
			rAttributes = m_occtShapeToAttributesMap[rkOcctShape];
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

	void AttributeManager::CopyAttributes(const TopoDS_Shape & rkOcctShape1, const TopoDS_Shape & rkOcctShape2)
	{
		std::map<std::string, Attribute::Ptr> attributes;
		bool isFound = FindAll(rkOcctShape1, attributes);
		if (isFound)
		{
			m_occtShapeToAttributesMap[rkOcctShape2] = std::map<std::string, Attribute::Ptr>(attributes);
		}
	}
}