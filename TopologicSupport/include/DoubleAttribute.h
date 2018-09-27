#pragma once

#include "Attribute.h"
#include "Utilities.h"

namespace TopologicSupport
{
	class DoubleAttribute : public Attribute
	{
	public:
		typedef std::shared_ptr<DoubleAttribute> Ptr;

		TOPOLOGIC_SUPPORT_API DoubleAttribute(const double kValue);
		virtual void* Value();
			
	protected:
		double m_value;
	};
}