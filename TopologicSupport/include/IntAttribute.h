#pragma once

#include "Attribute.h"
#include "Utilities.h"

namespace TopologicSupport
{
	class IntAttribute :public Attribute
	{
	public:
		typedef std::shared_ptr<IntAttribute> Ptr;

		TOPOLOGIC_SUPPORT_API IntAttribute(const long long int kValue);
		virtual void* Value();

	protected:
		long long int m_value;
	};
}