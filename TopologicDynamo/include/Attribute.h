#pragma once

#include <TopologicUtilities/include/Attribute.h>

namespace Topologic
{
	public ref class Attribute
	{
	public protected:
		property std::shared_ptr<TopologicUtilities::Attribute> UtilitiesAttribute
		{
			std::shared_ptr<TopologicUtilities::Attribute> get();
		}

		Attribute(const std::shared_ptr<TopologicUtilities::Attribute>& kpUtilitiesAttribute);
		~Attribute();

	protected:
		std::shared_ptr<TopologicUtilities::Attribute>* m_pUtilitiesAttribute;
	};
}