#include "DoubleAttribute.h"

namespace TopologicCore
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
