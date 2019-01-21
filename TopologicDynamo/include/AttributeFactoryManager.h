#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace TopologicUtilities
{
	class Attribute;
}

namespace Topologic
{
	ref class Topology;
	ref class AttributeFactory;

	ref class AttributeFactoryManager
	{
	public protected:
		static property AttributeFactoryManager^ Instance {
			AttributeFactoryManager^ get() { return %m_instance; }
		}

		void SetAttribute(Topology^ topology, String^ key, Object^ value);

		AttributeFactory^ GetFactory(const std::shared_ptr<TopologicUtilities::Attribute> kpSupportAttribute);

	private:
		AttributeFactoryManager();
		AttributeFactoryManager(const AttributeFactoryManager%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }

		AttributeFactory^ GetFactory(Object^ value);

		static AttributeFactoryManager m_instance;
		Dictionary<String^, AttributeFactory^>^ m_attributeFactoryDict = gcnew Dictionary<String^, AttributeFactory^>();
	};
}