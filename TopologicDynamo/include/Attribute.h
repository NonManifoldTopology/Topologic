#pragma once

#include <TopologicUtilities/include/Attribute.h>

namespace Topologic
{
	/// <summary>
	/// Attributes include the following classes: Attribute, AttributeFactory, AttributeFactoryManager, DoubleAttributeFactory, IntAttributeFactory and StringAttributeFactory.
	/// </summary>
	namespace Attributes
	{
		/// <summary>
		/// An Attribute to a Topology maps an ID to its value. 
		/// </summary>

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
}