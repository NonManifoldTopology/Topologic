#pragma once

using namespace System;
using namespace System::Collections::Generic;

namespace TopologicSupport
{
	class Attribute;
}

namespace Topologic
{
	ref class Topology;
	ref class AttributeFactory;

	ref class AttributeManager
	{
	public protected:
		static property AttributeManager^ Instance {
			AttributeManager^ get() { return %m_instance; }
		}

		void SetAttribute(Topology^ topology, String^ key, Object^ value);

		AttributeFactory^ GetFactory(const std::shared_ptr<TopologicSupport::Attribute> kpSupportAttribute);

	private:
		AttributeManager();
		AttributeManager(const AttributeManager%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }

		AttributeFactory^ GetFactory(Object^ value);

		static AttributeManager m_instance;
		Dictionary<String^, AttributeFactory^>^ m_attributeFactoryDict = gcnew Dictionary<String^, AttributeFactory^>();
	};
}