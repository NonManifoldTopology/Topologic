#pragma once

#include <TopologicUtilities/include/Attribute.h>

namespace Topologic
{
	/// <summary>
	/// An Attribute to a Topology maps an ID to its value. 
	/// </summary>

	public ref class Attribute
	{
	public protected:
		property std::shared_ptr<TopologicUtilities::Attribute> SupportAttribute
		{
			std::shared_ptr<TopologicUtilities::Attribute> get();
		}

		Attribute(const std::shared_ptr<TopologicUtilities::Attribute>& kpSupportAttribute);
		~Attribute();

	protected:
		std::shared_ptr<TopologicUtilities::Attribute>* m_pSupportAttribute;
	};
}