#include <Attribute.h>

namespace Topologic
{
	Attribute::Attribute(const std::shared_ptr<TopologicCore::Attribute>& kpCoreAttribute)
		: m_pCoreAttribute(new std::shared_ptr<TopologicCore::Attribute>(kpCoreAttribute))
	{

	}

	Attribute::~Attribute()
	{
		delete m_pCoreAttribute;
	}
}