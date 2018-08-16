#include <AttributeManager.h>
#include <AttributeMap.h>
#include <Attribute.h>

namespace TopologicSupport
{
	void AttributeManager::AddAttribute(const TopoDS_Shape& rkOcctShape, const std::string& rkKey, const Attribute::Ptr& kpAttribute)
	{
		if (m_occtShapeToAttributesMap.find(rkOcctShape) == m_occtShapeToAttributesMap.end())
		{
			m_occtShapeToAttributesMap.insert(std::pair<TopoDS_Shape, AttributeMap::Ptr>(rkOcctShape, std::make_shared<AttributeMap>()));
		}

		m_occtShapeToAttributesMap[rkOcctShape]->Add(rkKey, kpAttribute);
	}
	
	void AttributeManager::AddAttributeMap(const TopologicCore::Topology::Ptr& kpTopology, const std::shared_ptr<AttributeMap>& rkAttributeMap)
	{
		AddAttributeMap(kpTopology->GetOcctShape(), rkAttributeMap);
	}

	void AttributeManager::AddAttributeMap(const TopoDS_Shape& rkOcctShape, const std::shared_ptr<AttributeMap>& rkAttributeMap)
	{
		m_occtShapeToAttributesMap[rkOcctShape] = rkAttributeMap;
	}

	void AttributeManager::GetAttributeMap(const TopologicCore::Topology::Ptr& kpTopology, std::shared_ptr<AttributeMap>& rAttributeMap)
	{
		GetAttributeMap(kpTopology->GetOcctShape(), rAttributeMap);
	}

	void AttributeManager::GetAttributeMap(const TopoDS_Shape & rkOcctShape, std::shared_ptr<AttributeMap>& rAttributeMap)
	{
		rAttributeMap = m_occtShapeToAttributesMap[rkOcctShape];
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