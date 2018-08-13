#pragma once

#include "Attribute.h"
#include "Utilities.h"

namespace TopologicSupport
{
	class IntAttribute : public Attribute
	{
	public:
		typedef std::shared_ptr<IntAttribute> Ptr;

	public:
		TOPOLOGIC_SUPPORT_API IntAttribute(const __int64 kValue);
		virtual ~IntAttribute() {}

		virtual std::string GUID() const { return "302972be-b6e6-4ce4-b362-0f6adc55f09c"; };

		TOPOLOGIC_SUPPORT_API __int64 Value() const { return m_value; }

	protected:
		__int64 m_value;
	};
}