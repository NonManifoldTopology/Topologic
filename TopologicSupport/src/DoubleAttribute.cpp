#include <DoubleAttribute.h>

namespace TopologicSupport
{
	DoubleAttribute::DoubleAttribute(const double kValue)
		: m_value(kValue)
	{

	}

	void * DoubleAttribute::Value()
	{
		return &m_value;
	}
}
