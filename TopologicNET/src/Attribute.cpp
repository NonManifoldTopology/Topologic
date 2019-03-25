#include "Attribute.h"
#include "AttributeFactoryManager.h"
#include "AttributeFactory.h"

namespace Topologic
{
	namespace Attributes
	{
		TopologicCore::Attribute::Ptr Attribute::UtilitiesAttribute::get()
		{
			return *m_pUtilitiesAttribute;
		}

		Attribute::Attribute(const TopologicCore::Attribute::Ptr& kpUtilitiesAttribute)
			: m_pUtilitiesAttribute(new TopologicCore::Attribute::Ptr(kpUtilitiesAttribute))
		{

		}

		Attribute::~Attribute()
		{

		}
	}
}