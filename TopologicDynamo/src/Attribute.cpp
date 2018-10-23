#include <Attribute.h>

#include <AttributeManager.h>
#include <AttributeFactory.h>

namespace Topologic
{
	TopologicUtility::Attribute::Ptr Attribute::SupportAttribute::get()
	{
		return *m_pSupportAttribute;
	}

	Attribute::Attribute(const TopologicUtility::Attribute::Ptr& kpSupportAttribute)
		: m_pSupportAttribute(new TopologicUtility::Attribute::Ptr(kpSupportAttribute))
	{

	}

	Attribute::~Attribute()
	{
		// delete?
	}
}