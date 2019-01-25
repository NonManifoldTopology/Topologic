#include <Attribute.h>

#include <AttributeFactoryManager.h>
#include <AttributeFactory.h>

namespace Topologic
{
	TopologicUtilities::Attribute::Ptr Attribute::UtilitiesAttribute::get()
	{
		return *m_pUtilitiesAttribute;
	}

	Attribute::Attribute(const TopologicUtilities::Attribute::Ptr& kpUtilitiesAttribute)
		: m_pUtilitiesAttribute(new TopologicUtilities::Attribute::Ptr(kpUtilitiesAttribute))
	{

	}

	Attribute::~Attribute()
	{
		// delete?
	}
}