#pragma once

#include <TopologicCore/include/Attribute.h>

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
			property std::shared_ptr<TopologicCore::Attribute> UtilitiesAttribute
			{
				std::shared_ptr<TopologicCore::Attribute> get();
			}

			Attribute(const std::shared_ptr<TopologicCore::Attribute>& kpUtilitiesAttribute);
			~Attribute();

		protected:
			std::shared_ptr<TopologicCore::Attribute>* m_pUtilitiesAttribute;
		};
	}
}