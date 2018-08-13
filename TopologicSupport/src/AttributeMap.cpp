#include <AttributeMap.h>

namespace TopologicSupport
{
	AttributeMap::AttributeMap()
	{

	}

	AttributeMap::~AttributeMap()
	{
		Clear();
	}

	void AttributeMap::Add(const std::string & rkKey, const std::shared_ptr<Attribute>& kpAttribute)
	{
		m_stringToAttributeMap[rkKey] = kpAttribute;
	}

	void AttributeMap::Remove(const std::string & rkKey)
	{
		m_stringToAttributeMap.erase(rkKey);
	}

	bool AttributeMap::Find(const std::string & rkKey, std::shared_ptr<Attribute>& rAttribute)
	{
		if (m_stringToAttributeMap.find(rkKey) != m_stringToAttributeMap.end())
		{
			rAttribute = m_stringToAttributeMap[rkKey];
			return true;
		}

		return false;
	}

	void AttributeMap::Clear()
	{
		m_stringToAttributeMap.clear();
	}
}