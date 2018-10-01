#include <Attribute.h>

namespace Topologic
{
	TopologicSupport::Attribute::Ptr Attribute::SupportAttribute::get()
	{
		return *m_pSupportAttribute;
	}

	Attribute::Attribute(const TopologicSupport::Attribute::Ptr& kpSupportAttribute)
		: m_pSupportAttribute(new TopologicSupport::Attribute::Ptr(kpSupportAttribute))
	{

	}

	Attribute::~Attribute()
	{
		// delete?
	}
}