#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <string>

namespace TopologicSupport
{
	class StringAttribute : public Attribute
	{
	public:
		typedef std::shared_ptr<StringAttribute> Ptr;

		TOPOLOGIC_SUPPORT_API StringAttribute(const std::string& kValue);
		virtual void* Value();

	protected:
		std::string m_value;
	};
}