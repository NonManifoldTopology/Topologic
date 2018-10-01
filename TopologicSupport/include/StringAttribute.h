#pragma once

#include "Attribute.h"
#include "Utilities.h"

#include <string>

namespace TopologicSupport
{
	class StringAttributeGUID {
	public:
		static std::string Get()
		{
			return std::string("a915eb73-256d-42f3-a3e6-491b020c3954");
		}
	};

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