#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <string>

namespace TopologicCore
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

		TOPOLOGIC_API IntAttribute(const long long int kValue);
		virtual void* Value();
		long long int IntValue() { return m_value; }

	protected:
		long long int m_value;
	};
}