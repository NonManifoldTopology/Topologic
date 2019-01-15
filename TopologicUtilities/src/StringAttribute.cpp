#include <StringAttribute.h>

namespace TopologicUtilities
{
	StringAttribute::StringAttribute(const std::string& kValue)
		: m_value(kValue)
	{

	}

	void * StringAttribute::Value()
	{
		return &m_value;
	}
}