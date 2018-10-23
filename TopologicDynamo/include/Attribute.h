#pragma once

#include <TopologicUtility/include/Attribute.h>

namespace Topologic
{
	public ref class Attribute
	{
	public protected:
		property std::shared_ptr<TopologicUtility::Attribute> SupportAttribute
		{
			std::shared_ptr<TopologicUtility::Attribute> get();
		}

		Attribute(const std::shared_ptr<TopologicUtility::Attribute>& kpSupportAttribute);
		~Attribute();

	protected:
		std::shared_ptr<TopologicUtility::Attribute>* m_pSupportAttribute;
	};
}