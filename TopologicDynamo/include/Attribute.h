#pragma once

#include <TopologicSupport/include/Attribute.h>

namespace Topologic
{
	public ref class Attribute
	{
	public protected:
		property std::shared_ptr<TopologicSupport::Attribute> SupportAttribute
		{
			std::shared_ptr<TopologicSupport::Attribute> get();
		}

		Attribute(const std::shared_ptr<TopologicSupport::Attribute>& kpSupportAttribute);
		~Attribute();

	protected:
		std::shared_ptr<TopologicSupport::Attribute>* m_pSupportAttribute;
	};
}