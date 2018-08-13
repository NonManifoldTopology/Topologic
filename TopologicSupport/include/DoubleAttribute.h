#pragma once

#include "Attribute.h"
#include "Utilities.h"

namespace TopologicSupport
{
	class DoubleAttribute : public Attribute
	{
	public:
		typedef std::shared_ptr<DoubleAttribute> Ptr;

	public:
		TOPOLOGIC_SUPPORT_API DoubleAttribute(const double kValue);
		virtual ~DoubleAttribute() {}

		virtual std::string GUID() const { return "8ee5033c-928d-41c0-a6a1-007ee5cfcd53"; };

		TOPOLOGIC_SUPPORT_API double Value() const { return m_value; }

	protected:
		double m_value;
	};
}