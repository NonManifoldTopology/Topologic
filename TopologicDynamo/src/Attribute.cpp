#include <Attribute.h>

#include <AttributeFactoryManager.h>
#include <AttributeFactory.h>

namespace Topologic
{
	TopologicUtilities::Attribute::Ptr Attribute::SupportAttribute::get()
	{
		return *m_pSupportAttribute;
	}

	Attribute::Attribute(const TopologicUtilities::Attribute::Ptr& kpSupportAttribute)
		: m_pSupportAttribute(new TopologicUtilities::Attribute::Ptr(kpSupportAttribute))
	{

	}

	Attribute::~Attribute()
	{
		// delete?
	}
}