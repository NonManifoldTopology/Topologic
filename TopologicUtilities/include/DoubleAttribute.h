#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <string>

namespace TopologicUtilities
{
	class DoubleAttributeGUID {
	public:
		static std::string Get()
		{
			return std::string("881714ed-c3da-4aee-9c0b-36841dcb09bb");
		}
	};

	class DoubleAttribute : public Attribute
	{
	public:
		typedef std::shared_ptr<DoubleAttribute> Ptr;

		TOPOLOGIC_UTILITIES_API DoubleAttribute(const double kValue);
		virtual void* Value();
		double DoubleValue() { return m_value; }

	protected:
		double m_value;
	};
}