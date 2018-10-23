#include <StringAttribute.h>

namespace TopologicUtility
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