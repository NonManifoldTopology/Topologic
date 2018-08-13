#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <memory>
#include <map>

namespace TopologicSupport
{
	class AttributeMap
	{
	public:
		typedef std::shared_ptr<TopologicSupport::AttributeMap> Ptr;
		typedef std::map<std::string, Attribute::Ptr> Type;
		typedef std::string Key;
		typedef Attribute::Ptr Value;

	public:
		TOPOLOGIC_SUPPORT_API AttributeMap();
		TOPOLOGIC_SUPPORT_API ~AttributeMap();

		TOPOLOGIC_SUPPORT_API void Add(const std::string& rkKey, const std::shared_ptr<Attribute>& kpAttribute);

		TOPOLOGIC_SUPPORT_API void Remove(const std::string& rkKey);

		TOPOLOGIC_SUPPORT_API bool Find(const std::string& rkKey, std::shared_ptr<Attribute>& rAttribute);

		TOPOLOGIC_SUPPORT_API void Clear();

		friend Type::iterator begin(AttributeMap& rAttributeMap) {
			return rAttributeMap.m_stringToAttributeMap.begin();
		}

		friend Type::iterator end(AttributeMap& rAttributeMap) {
			return rAttributeMap.m_stringToAttributeMap.end();
		}

	protected:
		std::map<std::string, Attribute::Ptr> m_stringToAttributeMap;
	};
}