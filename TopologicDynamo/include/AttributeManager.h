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

	/// <summary>
	/// An AttributeFactoryManager stores the subclasses of AttributeFactory, identified by their IDs. This class is used to identify the attribute type. The ID is in the GUID format, represented as a String. A user-created AttributeFactory must be registered with a unique ID to this class.
	/// </summary>

	ref class AttributeManager
	{
	public protected:
		static property AttributeManager^ Instance {
			AttributeManager^ get() { return %m_instance; }
		}

		void SetAttribute(Topology^ topology, String^ key, Object^ value);

		AttributeFactory^ GetFactory(const std::shared_ptr<TopologicUtilities::Attribute> kpSupportAttribute);

	private:
		AttributeManager();
		AttributeManager(const AttributeManager%) { throw gcnew System::InvalidOperationException("singleton cannot be copy-constructed"); }

		AttributeFactory^ GetFactory(Object^ value);

		static AttributeManager m_instance;
		Dictionary<String^, AttributeFactory^>^ m_attributeFactoryDict = gcnew Dictionary<String^, AttributeFactory^>();
	};
}