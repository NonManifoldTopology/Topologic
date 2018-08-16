#pragma once

#include "Attribute.h"
#include "Utilities.h"

namespace TopologicSupport
{
	class StringAttribute : public Attribute
	{
	public:
		typedef std::shared_ptr<StringAttribute> Ptr;

	public:
		TOPOLOGIC_SUPPORT_API StringAttribute(const std::string& kValue);
		virtual ~StringAttribute() {}

		virtual std::string GUID() const { return "1b958f07-fd1c-4c21-a7c1-cbef964c16b9"; };

		TOPOLOGIC_SUPPORT_API std::string Value() const { return m_value; }

	protected:
		std::string m_value;
	};
}