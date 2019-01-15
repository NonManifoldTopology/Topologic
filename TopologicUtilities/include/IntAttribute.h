#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <string>

namespace TopologicUtilities
{
	class IntAttributeGUID {
	public:
		static std::string Get()
		{
			return std::string("1a8944c6-23e0-4607-9313-336f4ab3fd53");
		}
	};

	class IntAttribute :public Attribute
	{
	public:
		typedef std::shared_ptr<IntAttribute> Ptr;

		TOPOLOGIC_UTILITIES_API IntAttribute(const long long int kValue);
		virtual void* Value();

	protected:
		long long int m_value;
	};
}